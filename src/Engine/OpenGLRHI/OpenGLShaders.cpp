
#include "pch.h"

#include "OpenGLDynamicRHI.h"

namespace {

template<typename OpenGLShaderType>
OpenGLShaderType* CompileOpenGLShader(RHIShader* ShaderRHI, const std::vector<uint8_t>& Code)
{
	// TODO: Check for shader cache
	// TODO: integrity with global shader compiler

	GLint Resource = glCreateShader(OpenGLShaderType::TypeEnum);
	GLint length = (GLint)Code.size();
	const char* code = (const char*)Code.data();
	glShaderSource(Resource, 1, &code, &length);
	glCompileShader(Resource);

	GLint CompileStatus = GL_TRUE;
	glGetShaderiv(Resource, GL_COMPILE_STATUS, &CompileStatus);

	if (!CompileStatus) {
		char infoLog[512];
		glGetShaderInfoLog(Resource, 512, NULL, infoLog);
		std::cout << "Shader (type=" << OpenGLShaderType::TypeEnum << ") Compilation Failed:\n" << infoLog << std::endl;
	} else {
		std::cout << "Shader (type=" << OpenGLShaderType::TypeEnum << ") Compiled Successfully" << std::endl;
	}

	return new OpenGLShaderType(Resource);
}

template<typename RHIShaderType, typename OpenGLProxyType>
RHIShaderType* CreateProxyShader(const std::vector<uint8_t>& Code)
{
	return new OpenGLProxyType([&](RHIShaderType* ShaderRHI) {
		return CompileOpenGLShader<typename OpenGLProxyType::OpenGLType>(ShaderRHI, Code);
	});
}

template<class TOpenGLStage0, class TOpenGLStage1>
static void BindShaderStage(OpenGLLinkedProgramConfiguration& Config, ShaderStage NextStage, TOpenGLStage0*NextStageShader,
							ShaderStage PrevStage, TOpenGLStage1* PrevStageShader) {
	OpenGLLinkedProgramConfiguration::ShaderInfo& ShaderInfo = Config.Shaders[NextStage];

	ShaderInfo.Bindings = NextStageShader->Bindings;
	ShaderInfo.Resource = NextStageShader->GetGLResource();
}

static void MarkShaderParameterCachesDirty(OpenGLShaderParameterCache* ShaderParameters, bool UpdateCompute)
{
	const int32_t StageStart = UpdateCompute ? ShaderStage::SHADER_STAGE_COMPUTE : ShaderStage::SHADER_STAGE_VERTEX;
	const int32_t StageEnd = UpdateCompute ? ShaderStage::NUM_SHADER_STAGES : ShaderStage::NUM_NON_COMPUTE_SHADER_STAGES;
	for (int32_t Stage = StageStart; Stage < StageEnd; ++Stage) {
		ShaderParameters[Stage].MarkAllDirty();
	}
}

static void ConfigureShaderStages(OpenGLLinkedProgram* LinkedProgram)
{
	const OpenGLLinkedProgramConfiguration& Config = LinkedProgram->Config;

	if (Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_VERTEX,
			0
		);
	}

	if (Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_PIXEL,
			Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Bindings.NumUniformBuffers
		);
	}

	if (Config.Shaders[ShaderStage::SHADER_STAGE_GEOMETRY].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_GEOMETRY,
			Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Bindings.NumUniformBuffers
		);
	}

	if (Config.Shaders[ShaderStage::SHADER_STAGE_HULL].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_HULL,
			Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_GEOMETRY].Bindings.NumUniformBuffers
		);
	}

	if (Config.Shaders[ShaderStage::SHADER_STAGE_DOMAIN].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_DOMAIN,
			Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_GEOMETRY].Bindings.NumUniformBuffers +
			Config.Shaders[ShaderStage::SHADER_STAGE_HULL].Bindings.NumUniformBuffers
		);
	}

	if (Config.Shaders[ShaderStage::SHADER_STAGE_COMPUTE].Resource) {
		LinkedProgram->ConfigureShaderStage(
			ShaderStage::SHADER_STAGE_COMPUTE,
			0
		);
	}
}

static OpenGLLinkedProgram* LinkProgram(OpenGLLinkedProgramConfiguration Config)
{
	// TODO(slavi) check for cache based on the config
	// we might have a program that is cached in that way

	// Create program and attach shaders
	GLuint Program = glCreateProgram();

	{
		if (Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Resource) {
			glAttachShader(Program, Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Resource);
		}

		if (Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Resource) {
			glAttachShader(Program, Config.Shaders[ShaderStage::SHADER_STAGE_PIXEL].Resource);
		}

		glLinkProgram(Program);
	}

	// todo(slavi) verify linkage

	// create a linked program and use it to process its stages
	OpenGLLinkedProgram* LinkedProgram = new OpenGLLinkedProgram(Config, Program);
	glUseProgram(Program);

	ConfigureShaderStages(LinkedProgram);

	return LinkedProgram;
}

static char* SetIndex(char* Str, int32_t MemberOffset, int32_t Index)
{
	Str += MemberOffset;
	if (Index >= 10) {
		*Str++ = '0' + (char)(Index / 10);
	}
	*Str++ = '0' + (char)(Index % 10);
	*Str = '\0';
	return Str;
}

static std::map<GLuint, std::map<std::string, int64_t>>& GetOpenGLUniformBlockLocations()
{
	static std::map<GLuint, std::map<std::string, int64_t>> UniformBlockLocations;
	return UniformBlockLocations;
}

static GLuint GetOpenGLProgramUniformBlockIndex(GLuint Program, const char* UniformBlockName)
{
	std::map<std::string, int64_t>& Locations = GetOpenGLUniformBlockLocations()[Program];

	auto it = Locations.find(UniformBlockName);
	int64_t* Location = it != Locations.end() ? &(*it).second : nullptr;
	if (Location) {
		return *Location;
	} else {
		it = Locations.emplace(UniformBlockName, glGetUniformBlockIndex(Program, UniformBlockName)).first;
		return (*it).second;
	}
}

static std::map<GLuint, std::map<int64_t, int64_t>>& GetOpenGLUniformBlockBindings()
{
	static std::map<GLuint, std::map<int64_t, int64_t>> UniformBlockBindings;
	return UniformBlockBindings;
}

static void GetOpenGLProgramUniformBlockBinding(GLuint Program, GLuint UniformBlockIndex, GLuint UniformBlockBinding)
{
	std::map<int64_t, int64_t>& Bindings = GetOpenGLUniformBlockBindings()[Program];
	auto it = Bindings.find(UniformBlockIndex);

	int64_t* Bind = it != Bindings.end() ? &(*it).second : nullptr;
	if (!Bind) {
		Bind = &((*Bindings.emplace(UniformBlockIndex, -1).first).second);
	}

	if (*Bind != static_cast<int64_t>(UniformBlockBinding)) {
		*Bind = static_cast<int64_t>(UniformBlockBinding);
		glUniformBlockBinding(Program, UniformBlockIndex, UniformBlockBinding);
	}
}

}

OpenGLShaderParameterCache::OpenGLShaderParameterCache()
	: GlobalUniformArraySize(-1)
{
	for (int32_t ArrayIndex = 0; ArrayIndex < PACKED_TYPEINDEX_MAX; ++ArrayIndex) {
		PackedGlobalUniformDirty[ArrayIndex].StartVector = 0;
		PackedGlobalUniformDirty[ArrayIndex].NumVectors = 0;
	}
}

OpenGLShaderParameterCache::~OpenGLShaderParameterCache()
{
	if (GlobalUniformArraySize > 0) {
		free(PackedUniformsScratch[0]);
		free(PackedGlobalUniforms[0]);
	}

	GlobalUniformArraySize = -1;
}

void OpenGLShaderParameterCache::InitializeResources(int32_t UniformArraySize)
{
	if (GlobalUniformArraySize != -1) {
		return;
	}

	// Uniform arrays have to be multiples of float4s.
	UniformArraySize = Align(UniformArraySize, 4 * sizeof(float));

	PackedGlobalUniforms[0] = (uint8_t*)malloc(UniformArraySize * PACKED_TYPEINDEX_MAX);
	PackedUniformsScratch[0] = (uint8_t*)malloc(UniformArraySize * PACKED_TYPEINDEX_MAX);

	memset(PackedGlobalUniforms[0], 0, UniformArraySize * PACKED_TYPEINDEX_MAX);
	memset(PackedUniformsScratch[0], 0, UniformArraySize * PACKED_TYPEINDEX_MAX);

	for (int32_t ArrayIndex = 1; ArrayIndex < PACKED_TYPEINDEX_MAX; ++ArrayIndex) {
		PackedGlobalUniforms[ArrayIndex] = PackedGlobalUniforms[ArrayIndex - 1] + UniformArraySize;
		PackedUniformsScratch[ArrayIndex] = PackedUniformsScratch[ArrayIndex - 1] + UniformArraySize;
	}
	GlobalUniformArraySize = UniformArraySize;

	for (int32_t ArrayIndex = 0; ArrayIndex < PACKED_TYPEINDEX_MAX; ++ArrayIndex) {
		PackedGlobalUniformDirty[ArrayIndex].StartVector = 0;
		PackedGlobalUniformDirty[ArrayIndex].NumVectors = UniformArraySize / (4 * sizeof(float));
	}
}

void OpenGLShaderParameterCache::MarkAllDirty() {
	for (int32_t ArrayIndex = 0; ArrayIndex < PACKED_TYPEINDEX_MAX; ++ArrayIndex) {
		PackedGlobalUniformDirty[ArrayIndex].StartVector = 0;
		PackedGlobalUniformDirty[ArrayIndex].NumVectors = GlobalUniformArraySize / (4 * sizeof(float));
	}
}

void OpenGLShaderParameterCache::Set(uint32_t BufferIndexName, uint32_t ByteOffset, uint32_t NumBytes, const void* NewValues) {
	uint32_t BufferIndex = PackedTypeNameToTypeIndex(BufferIndexName);
	size_t SizeOfFloat4 = (4 * sizeof(float));
	PackedGlobalUniformDirty[BufferIndex].MarkDirtyRange(ByteOffset / SizeOfFloat4, (NumBytes + SizeOfFloat4 - 1) / SizeOfFloat4);
	memcpy(PackedGlobalUniforms[BufferIndex] + ByteOffset, NewValues, NumBytes);
}

void OpenGLShaderParameterCache::CommitPackedGlobals(const OpenGLLinkedProgram* LinkedProgram, int32_t Stage)
{
	const uint32_t BytesPerRegister = 16;

	const OpenGLLinkedProgram::PackedUniformInfoVec& PackedUniforms = LinkedProgram->StagePackedUniformInfo[Stage].PackedUniforms;
	const PackedArrayInfoVec& PackedArrays = LinkedProgram->Config.Shaders[Stage].Bindings.PackedGlobalArrays;

	for (size_t PackedUniform = 0; PackedUniform < PackedUniforms.size(); ++PackedUniform) {
		const OpenGLLinkedProgram::PackedUniformInfo& UniformInfo = PackedUniforms[PackedUniform];
		GLint Location = UniformInfo.Location;
		const uint32_t ArrayIndex = UniformInfo.Index;
		if (Location >= 0 && // Probably this uniform array was optimized away in a linked program
			PackedGlobalUniformDirty[ArrayIndex].NumVectors > 0) {
			const uint32_t NumVectors = PackedArrays[PackedUniform].Size / BytesPerRegister;
			const void* UniformData = PackedGlobalUniforms[ArrayIndex];

			const uint32_t StartVector = PackedGlobalUniformDirty[ArrayIndex].StartVector;
			int32_t NumDirtyVectors = std::min(PackedGlobalUniformDirty[ArrayIndex].NumVectors, NumVectors - StartVector);

			UniformData = (uint8_t*)UniformData + StartVector * sizeof(float) * 4;
			Location += StartVector;
			switch (UniformInfo.Index) {
				case PACKED_TYPEINDEX_HIGHP:
				case PACKED_TYPEINDEX_MEDIUMP:
				case PACKED_TYPEINDEX_LOWP:
					glUniform4fv(Location, NumDirtyVectors, (GLfloat*)UniformData);
					break;

				case PACKED_TYPEINDEX_INT:
					glUniform4iv(Location, NumDirtyVectors, (GLint*)UniformData);
					break;

				case PACKED_TYPEINDEX_UINT:
					// todo(slavi) uint might not be supported
					glUniform4uiv(Location, NumDirtyVectors, (GLuint*)UniformData);
					break;
			}

			PackedGlobalUniformDirty[ArrayIndex].StartVector = 0;
			PackedGlobalUniformDirty[ArrayIndex].NumVectors = 0;
		}
	}
}

void OpenGLShaderParameterCache::CommitPackedUniformBuffers(OpenGLLinkedProgram* LinkedProgram, int32_t Stage, RHIUniformBufferRef* UniformBuffers, const std::vector<UniformBufferCopyInfo>& UniformBuffersCopyInfo)
{
	// todo; es2 doesn't support uniform buffers
	// so this is done to emulate it
}

void OpenGLLinkedProgram::VerifyUniformBlockBindings(int Stage, uint32_t FirstUniformBuffer)
{
	// TODO(slavi)
}

void OpenGLLinkedProgram::ConfigureShaderStage(int Stage, uint32_t FirstUniformBuffer)
{
	char NameBuffer[10] { 0 };

	NameBuffer[0] = ShaderStageIndexToTypeName(Stage);

	// Bind Global uniform arrays (vu_h, pu_i, etc)
	{
		NameBuffer[1] = 'u';
		NameBuffer[2] = '_';
		NameBuffer[3] = 0;
		NameBuffer[4] = 0;

		PackedUniformInfoVec PackedUniformInfos;
		for (uint8_t Index = 0; Index < PACKED_TYPEINDEX_MAX; ++Index) {
			uint8_t ArrayIndexType = PackedTypeIndexToTypeName(Index);
			NameBuffer[3] = ArrayIndexType;
			GLint Location = glGetUniformLocation(Program, NameBuffer);
			if ((int32_t)Location != -1) {
				PackedUniformInfo Info = { Location, ArrayIndexType, Index };
				PackedUniformInfos.push_back(Info);
			}
		}

		SortPackedUniformInfos(PackedUniformInfos, Config.Shaders[Stage].Bindings.PackedGlobalArrays,
							   StagePackedUniformInfo[Stage].PackedUniforms);
	}

	// Bind uniform buffer packed arrays (vc0_h, pc2_i, etc)
	{
		NameBuffer[1] = 'c';
		NameBuffer[2] = 0;
		NameBuffer[3] = 0;
		NameBuffer[4] = 0;
		NameBuffer[5] = 0;
		NameBuffer[6] = 0;
		for (uint8_t UB = 0; UB < Config.Shaders[Stage].Bindings.NumUniformBuffers; ++UB) {
			PackedUniformInfoVec PackedBuffers;
			char* Str = SetIndex(NameBuffer, 2, UB);
			*Str++ = '_';
			Str[1] = 0;
			for (uint8_t Index = 0; Index < PACKED_TYPEINDEX_MAX; ++Index) {
				uint8_t ArrayIndexType = PackedTypeIndexToTypeName(Index);
				Str[0] = ArrayIndexType;
				GLint Location = glGetUniformLocation(Program, NameBuffer);
				if ((int32_t)Location != -1) {
					PackedUniformInfo Info = { Location, ArrayIndexType, Index };
					PackedBuffers.push_back(Info);
				}
			}

			StagePackedUniformInfo[Stage].PackedUniformBuffers.push_back(PackedBuffers);
		}
	}

	// Reserve and setup Space for Emulated Uniform Buffers
	StagePackedUniformInfo[Stage].LastEmulatedUniformBufferSet.clear();
	StagePackedUniformInfo[Stage].LastEmulatedUniformBufferSet = std::vector<uint32_t>(Config.Shaders[Stage].Bindings.NumUniformBuffers, 0);

	// TODO(slavi): bind samplers
	// TODO(slavi): bind UAVs

	// Bind uniform buffers.
	if (OpenGLSpecs::SupportsUniformBuffers()) {
		NameBuffer[1] = 'b';
		NameBuffer[2] = 0;
		NameBuffer[3] = 0;
		NameBuffer[4] = 0;
		for (int32_t BufferIndex = 0; BufferIndex < Config.Shaders[Stage].Bindings.NumUniformBuffers; ++BufferIndex) {
			SetIndex(NameBuffer, 2, BufferIndex);
			GLint Location = GetOpenGLProgramUniformBlockIndex(Program, NameBuffer);
			if (Location >= 0) {
				GetOpenGLProgramUniformBlockBinding(Program, Location, FirstUniformBuffer + BufferIndex);
			}
		}
	}
}

void OpenGLLinkedProgram::SortPackedUniformInfos(const PackedUniformInfoVec& ReflectedUniformInfos, const PackedArrayInfoVec& PackedGlobalArrays, PackedUniformInfoVec& OutPackedUniformInfos)
{
	OutPackedUniformInfos.reserve(PackedGlobalArrays.size());
	for (size_t Index = 0; Index < PackedGlobalArrays.size(); ++Index) {
		auto& PackedArray = PackedGlobalArrays[Index];
		PackedUniformInfo OutInfo = { -1, PackedArray.TypeName, PACKED_TYPEINDEX_MAX };

		for (size_t FindIndex = 0; FindIndex < ReflectedUniformInfos.size(); ++FindIndex) {
			auto& ReflectedInfo = ReflectedUniformInfos[FindIndex];
			if (ReflectedInfo.ArrayType == PackedArray.TypeName) {
				OutInfo = ReflectedInfo;
				break;
			}
		}

		OutPackedUniformInfos.push_back(OutInfo);
	}
}

OpenGLBoundShaderState::OpenGLBoundShaderState(
	OpenGLLinkedProgram* LinkedProgram,
	OpenGLVertexDeclaration* VertexDeclaration,
	RHIVertexShader* VertexShaderRHI,
	RHIPixelShader* PixelShaderRHI)
	: mLinkedProgram(LinkedProgram)
	, mVertexDeclaration(VertexDeclaration)
{
	mVertexShaderProxy = StaticResourceCast(VertexShaderRHI);
	mPixelShaderProxy = StaticResourceCast(PixelShaderRHI);
}

RHIVertexShaderRef OpenGLDynamicRHI::RHICreateVertexShader(const std::vector<uint8_t>& Code)
{
	return CreateProxyShader<RHIVertexShader, OpenGLVertexShaderProxy>(Code);
}

RHIPixelShaderRef OpenGLDynamicRHI::RHICreatePixelShader(const std::vector<uint8_t>& Code)
{
	return CreateProxyShader<RHIPixelShader, OpenGLPixelShaderProxy>(Code);
}

RHIBoundShaderStateRef OpenGLDynamicRHI::RHICreateBoundShaderState(
	RHIVertexDeclaration* VertexDeclarationRHI,
	RHIVertexShader* VertexShaderRHI,
	RHIPixelShader* PixelShaderRHI)
{
	assert(VertexShaderRHI && PixelShaderRHI);

	// TODO: Cache this state, we shouldn't create a shader program
	// everytime for the same combination

	OpenGLVertexShader* VertexShader = StaticResourceCast(VertexShaderRHI)->GetGLResourceObject();
	OpenGLPixelShader* PixelShader = StaticResourceCast(PixelShaderRHI)->GetGLResourceObject();

	OpenGLLinkedProgramConfiguration Config;

	Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Bindings = VertexShader->Bindings;
	Config.Shaders[ShaderStage::SHADER_STAGE_VERTEX].Resource = VertexShader->GetGLResource();

	BindShaderStage(Config, ShaderStage::SHADER_STAGE_PIXEL, PixelShader, ShaderStage::SHADER_STAGE_VERTEX, VertexShader);

	GetContextStateForCurrentContext().Program = -1;
	MarkShaderParameterCachesDirty(mPendingState.ShaderParameters, false);
	mPendingState.LinkedProgramAndDirtyFlag = nullptr;

	OpenGLLinkedProgram* LinkedProgram = LinkProgram(Config);

	// TODO: error check for linking success, compile status, ...
	OpenGLVertexDeclaration* VertexDeclaration = StaticResourceCast(VertexDeclarationRHI);
	OpenGLBoundShaderState* BoundShaderState = new OpenGLBoundShaderState(
		LinkedProgram,
		VertexDeclaration,
		VertexShaderRHI,
		PixelShaderRHI
	);

	return BoundShaderState;
}



void OpenGLDynamicRHI::BindPendingShaderState(OpenGLContextState& ContextState)
{
	OpenGLLinkedProgram* LinkedProgram = mPendingState.BoundShaderState->GetLinkedProgram();
	GLuint PendingProgram = LinkedProgram->Program;

	bool ForceUniformBindingUpdate = false;

	if (ContextState.Program != PendingProgram) {
		glUseProgram(PendingProgram);
		ContextState.Program = PendingProgram;
		MarkShaderParameterCachesDirty(mPendingState.ShaderParameters, false);
		mPendingState.LinkedProgramAndDirtyFlag = nullptr;

		// TODO(slavi)
		//ForceUniformBindingUpdate = OpenGLSpecs::SupportsSeparateShaderObjects();
	}

	// uniform buffers if supported
	if (OpenGLSpecs::SupportsUniformBuffers()) {
		int32_t NextUniformBufferIndex = 0;

		int32_t NumUniformBuffers[SF_Compute];
		mPendingState.BoundShaderState->GetNumUniformBuffers(NumUniformBuffers);

		mPendingState.BoundShaderState->GetLinkedProgram()->VerifyUniformBlockBindings(ShaderStage::SHADER_STAGE_VERTEX, NextUniformBufferIndex);
		BindUniformBufferBase(
			ContextState,
			NumUniformBuffers[SF_Vertex],
			mPendingState.BoundUniformBuffers[SF_Vertex],
			NextUniformBufferIndex, ForceUniformBindingUpdate);
		NextUniformBufferIndex += NumUniformBuffers[SF_Vertex];

		mPendingState.BoundShaderState->GetLinkedProgram()->VerifyUniformBlockBindings(ShaderStage::SHADER_STAGE_PIXEL, NextUniformBufferIndex);
		BindUniformBufferBase(
			ContextState,
			NumUniformBuffers[SF_Pixel],
			mPendingState.BoundUniformBuffers[SF_Pixel],
			NextUniformBufferIndex, ForceUniformBindingUpdate);
		NextUniformBufferIndex += NumUniformBuffers[SF_Pixel];

		// TODO: other shader stages (geometry, hull, ...)
	}
}
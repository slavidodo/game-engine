
#include "pch.h"

#include "OpenGLDynamicRHI.h"

#include "../Rendering/RenderUtils.h"

// todo(slavi) include all core headers in one file
#include "../Math/MathUtility.h"

extern int32_t volatile gCurrentTextureMemorySize;
extern int32_t volatile gCurrentRendertargetMemorySize;

namespace
{

static bool ShouldCountAsTextureMemory(uint32_t Flags)
{
	return (Flags & (TexCreate_RenderTargetable | TexCreate_ResolveTargetable | TexCreate_DepthStencilTargetable)) == 0;
}

void OpenGLTextureAllocated(RHITexture* Texture, uint32_t Flags)
{
	int32_t TextureSize = 0;
	bool IsRenderTarget = !ShouldCountAsTextureMemory(Flags);

	if (OpenGLTextureCube* TextureCube = (OpenGLTextureCube*)Texture->GetTextureCube()) {
		if (TextureCube->IsMemorySizeSet()) {
			return; // already set this up on RT
		}

		TextureSize = CalcTextureSize(TextureCube->GetSize(), TextureCube->GetSize(), TextureCube->GetFormat(), TextureCube->GetNumMips());
		TextureSize *= TextureCube->GetArraySize() * (TextureCube->GetArraySize() == 1 ? 6 : 1);
		TextureCube->SetMemorySize(TextureSize);
		TextureCube->SetIsPowerOfTwo(Math::IsPowerOfTwo(TextureCube->GetSizeX()) && Math::IsPowerOfTwo(TextureCube->GetSizeY()));
	} else if (OpenGLTexture2D* Texture2D = (OpenGLTexture2D*)Texture->GetTexture2D()) {
		if (Texture2D->IsMemorySizeSet()) {
			return; // already set this up on RT
		}
		TextureSize = CalcTextureSize(Texture2D->GetSizeX(), Texture2D->GetSizeY(), Texture2D->GetFormat(), Texture2D->GetNumMips()) * Texture2D->GetNumSamples();
		Texture2D->SetMemorySize(TextureSize);
		Texture2D->SetIsPowerOfTwo(Math::IsPowerOfTwo(Texture2D->GetSizeX()) && Math::IsPowerOfTwo(Texture2D->GetSizeY()));
	} else if (OpenGLTexture3D* Texture3D = (OpenGLTexture3D*)Texture->GetTexture3D()) {
		if (Texture3D->IsMemorySizeSet()) {
			return; // already set this up on RT
		}
		TextureSize = CalcTextureSize3D(Texture3D->GetSizeX(), Texture3D->GetSizeY(), Texture3D->GetSizeZ(), Texture3D->GetFormat(), Texture3D->GetNumMips());
		Texture3D->SetMemorySize(TextureSize);
		Texture3D->SetIsPowerOfTwo(Math::IsPowerOfTwo(Texture3D->GetSizeX()) && Math::IsPowerOfTwo(Texture3D->GetSizeY()) && Math::IsPowerOfTwo(Texture3D->GetSizeZ()));
	} else if (OpenGLTexture2DArray* Texture2DArray = (OpenGLTexture2DArray*)Texture->GetTexture2DArray()) {
		if (Texture2DArray->IsMemorySizeSet()) {
			return; // already set this up on RT
		}
		TextureSize = Texture2DArray->GetSizeZ() * CalcTextureSize(Texture2DArray->GetSizeX(), Texture2DArray->GetSizeY(), Texture2DArray->GetFormat(), Texture2DArray->GetNumMips());
		Texture2DArray->SetMemorySize(TextureSize);
		Texture2DArray->SetIsPowerOfTwo(Math::IsPowerOfTwo(Texture2DArray->GetSizeX()) && Math::IsPowerOfTwo(Texture2DArray->GetSizeY()));
	} else {
		throw std::runtime_error("unhandled texture type."); // handle more texture types?
	}

	if (IsRenderTarget) {
		gCurrentRendertargetMemorySize += Align(TextureSize, 1024) / 1024;
	} else {
		gCurrentTextureMemorySize += Align(TextureSize, 1024) / 1024;
	}
}

}

RHITexture* OpenGLDynamicRHI::CreateOpenGLRHITextureOnly(const uint32_t SizeX, const uint32_t SizeY, const bool IsCubeTexture, const bool IsArrayTexture, const bool IsExternal, uint8_t& Format, uint32_t& NumMips, uint32_t& NumSamples, const uint32_t ArraySize, uint32_t& Flags)
{
	if (NumMips == 0) {
		if (NumSamples <= 1) {
			NumMips = FindMaxMipmapLevel(SizeX, SizeY);
		} else {
			NumMips = 1;
		}
	}

	// Move NumSamples to on-chip MSAA if supported
	uint32_t NumSamplesTileMem = 1;
	GLint MaxSamplesTileMem = OpenGLSpecs::GetMaxMSAASamplesTileMem(); /* RHIs which do not support tiled GPU MSAA return 0 */
	if (MaxSamplesTileMem > 0) {
		NumSamplesTileMem = Math::Min<uint32_t>(NumSamples, MaxSamplesTileMem);
		NumSamples = 1;
	}

	if ((Flags & TexCreate_RenderTargetable) && Format == PF_B8G8R8A8 && !OpenGLSpecs::SupportsBGRA8888RenderTarget()) {
		// Some android devices does not support BGRA as a color attachment
		Format = PF_R8G8B8A8;
	}

	// TODO(slavi) ES2 doesn't support sRGB
	if (false) {
		// Remove sRGB read flag when not supported
		Flags &= ~TexCreate_SRGB;
	}

	GLenum Target = GL_NONE;
	if (IsCubeTexture) {
		if (OpenGLSpecs::SupportsTexture3D()) {
			Target = IsArrayTexture ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_CUBE_MAP;
		} else {
			// opengl doesn't support arrays in cubemaps
			Target = GL_TEXTURE_CUBE_MAP;
		}
	}
#if PLATFORM_ANDROID && !PLATFORM_LUMINGL4
	else if (IsExternal) {
		if (OpenGLSpecs::SupportsImageExternal()) {
			Target = GL_TEXTURE_EXTERNAL_OES;
		} else {
			// Fall back to a regular 2d texture if we don't have support. Texture samplers in the shader will also fall back to a regular sampler2D.
			Target = GL_TEXTURE_2D;
		}
	}
#endif
	else {
		Target = (NumSamples > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

		// @todo: refactor 2d texture array support here?
	}

	RHITexture* Result;
	// Allocate RHIResource with empty GL values.
	if (IsCubeTexture) {
		Result = new OpenGLTextureCube(this, 0, Target, -1, SizeX, SizeY, 0, NumMips, 1, 1, ArraySize, (EPixelFormat)Format, true, false, Flags, nullptr);
	} else {
		Result = new OpenGLTexture2D(this, 0, Target, -1, SizeX, SizeY, 0, NumMips, NumSamples, NumSamplesTileMem, 1, (EPixelFormat)Format, false, false, Flags, nullptr);
	}
	OpenGLTextureAllocated(Result, Flags);
	return Result;
}

RHITexture* OpenGLDynamicRHI::CreateOpenGLTexture(uint32_t SizeX, uint32_t SizeY, bool bCubeTexture, bool bArrayTexture, bool bIsExternal, uint8_t Format, uint32_t NumMips, uint32_t NumSamples, uint32_t ArraySize, uint32_t Flags)
{
	// Fill in the GL resources.
	RHITexture* Texture = CreateOpenGLRHITextureOnly(SizeX, SizeY, bCubeTexture, bArrayTexture, bIsExternal, Format, NumMips, NumSamples, ArraySize, Flags);

	InitializeGLTexture(Texture, SizeX, SizeY, bCubeTexture, bArrayTexture, bIsExternal, Format, NumMips, NumSamples, ArraySize, Flags);
	return Texture;
}

RHITexture2DRef OpenGLDynamicRHI::RHICreateTexture2D(uint32_t SizeX, uint32_t SizeY, uint8_t Format, uint32_t NumMips, uint32_t NumSamples, uint32_t Flags)
{
	return (RHITexture2D*)CreateOpenGLTexture(SizeX, SizeY, false, false, false, Format, NumMips, NumSamples, 1, Flags);
}

RHITexture2DRef OpenGLDynamicRHI::RHICreateTexture2DFromResource(EPixelFormat Format, uint32_t SizeX, uint32_t SizeY, uint32_t NumMips, uint32_t NumSamples, uint32_t NumSamplesTileMem, uint32_t Flags, GLuint Resource, uint32_t TexCreateFlags)
{
	OpenGLTexture2D* Texture2D = new OpenGLTexture2D(
		this,
		Resource,
		(NumSamples > 1) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
		GL_NONE,
		SizeX,
		SizeY,
		0,
		NumMips,
		NumSamples,
		NumSamplesTileMem,
		1,
		Format,
		false,
		false,
		TexCreateFlags,
		nullptr);

	Texture2D->SetAliased(true);
	OpenGLTextureAllocated(Texture2D, TexCreateFlags);
	return Texture2D;
}
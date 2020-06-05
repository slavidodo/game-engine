
#ifndef ENGINE_RENDERING_SHADERPARAMETERMACROS_H
#define ENGINE_RENDERING_SHADERPARAMETERMACROS_H

template<typename T, int32_t Alignment>
class TAlignedTypedef;

#define IMPLEMENT_ALIGNED_TYPE(Alignment) \
	template<typename T> \
	class TAlignedTypedef<T,Alignment> \
	{ \
	public: \
		typedef __declspec(align(Alignment)) T Type; \
	};

IMPLEMENT_ALIGNED_TYPE(1);
IMPLEMENT_ALIGNED_TYPE(2);
IMPLEMENT_ALIGNED_TYPE(4);
IMPLEMENT_ALIGNED_TYPE(8);
IMPLEMENT_ALIGNED_TYPE(16);
#undef IMPLEMENT_ALIGNED_TYPE

#ifdef ARCH_64

/** Fixed 8bytes sized and aligned pointer for shader parameters. */
template<typename PtrType>
using TAlignedShaderParameterPtr = typename TAlignedTypedef<PtrType, SHADER_PARAMETER_POINTER_ALIGNMENT>::Type;

#else //!ARCH_64

/** Fixed 8bytes sized pointer for shader parameters. */
template<typename PtrType>
class alignas(sizeof(uint64_t)) TAlignedShaderParameterPtr
{
public:
	TAlignedShaderParameterPtr()
	{ }

	TAlignedShaderParameterPtr(const PtrType & Other)
		: Ref(Other)
	{ }

	TAlignedShaderParameterPtr(const TAlignedShaderParameterPtr<PtrType> & Other)
		: Ref(Other.Ref)
	{ }

	inline void operator=(const PtrType& Other) {
		Ref = Other;
	}

	inline operator PtrType& () {
		return Ref;
	}

	inline operator const PtrType& () const {
		return Ref;
	}

	inline const PtrType& operator->() const {
		return Ref;
	}

private:
	PtrType Ref;
	uint32_t _Padding;

	static_assert(sizeof(PtrType) == sizeof(void*), "T should be a pointer.");
};

#endif // !ARCH_64

template<typename TBuffer>
class UniformBufferRef : public RHIUniformBufferRef
{
public:
	UniformBufferRef()
		: RHIUniformBufferRef(nullptr) {
	}

	UniformBufferRef(RHIUniformBufferRef Reference)
		: RHIUniformBufferRef(Reference) {
	}

	static UniformBufferRef<TBuffer> CreateUniformBufferImmediate(const TBuffer& Value, EUniformBufferUsage Usage, EUniformBufferValidation Validation = EUniformBufferValidation::ValidateResources) {
		return UniformBufferRef<TBuffer>(gDynamicRHI->RHICreateUniformBuffer(&Value, TBuffer::GetLayout(), Usage, Validation));
	}

	void UpdateUniformBufferImmediate(TBuffer buffer) {
		gDynamicRHI->RHIUpdateUniformBuffer(*this, &buffer);
	}

private:
};

template<typename TypeParameter>
struct TShaderParameterTypeInfo
{
	/** Defines what the type actually is. */
	static constexpr EUniformBufferBaseType BaseType = UBMT_INVALID;

	/** Defines the number rows and columns for vector or matrix based data typed. */
	static constexpr int32_t NumRows = 1;
	static constexpr int32_t NumColumns = 1;

	/** Defines the number of elements in an array fashion. 0 means this is not a TStaticArray,
	 * which therefor means there is 1 element.
	 */
	static constexpr int32_t NumElements = 0;

	/** Defines the alignment of the elements in bytes. */
	static constexpr int32_t Alignment = alignof(TypeParameter);

	/** Defines whether this element is stored in constant buffer or not.
	 * This informations is usefull to ensure at compile time everything in the
	 * structure get defined at the end of the structure, to reduce as much as possible
	 * the size of the constant buffer.
	 */
	static constexpr bool IsStoredInConstantBuffer = true;

	/** Type that is actually alligned. */
	using TAlignedType = TypeParameter;
};

template<>
struct TShaderParameterTypeInfo<glm::fvec3>
{
	static constexpr EUniformBufferBaseType BaseType = UBMT_FLOAT32;
	static constexpr int32_t NumRows = 1;
	static constexpr int32_t NumColumns = 3;
	static constexpr int32_t NumElements = 0;
	static constexpr int32_t Alignment = 16;
	static constexpr bool bIsStoredInConstantBuffer = true;

	using TAlignedType = TAlignedTypedef<glm::fvec3, Alignment>::Type;
};

template<>
struct TShaderParameterTypeInfo<glm::fvec4>
{
	static constexpr EUniformBufferBaseType BaseType = UBMT_FLOAT32;
	static constexpr int32_t NumRows = 1;
	static constexpr int32_t NumColumns = 4;
	static constexpr int32_t NumElements = 0;
	static constexpr int32_t Alignment = 16;
	static constexpr bool bIsStoredInConstantBuffer = true;

	using TAlignedType = TAlignedTypedef<glm::fvec4, Alignment>::Type;
};


template<>
struct TShaderParameterTypeInfo<glm::fmat3x3>
{
	static constexpr EUniformBufferBaseType BaseType = UBMT_FLOAT32;
	static constexpr int32_t NumRows = 3;
	static constexpr int32_t NumColumns = 3;
	static constexpr int32_t NumElements = 0;
	static constexpr int32_t Alignment = 16;
	static constexpr bool bIsStoredInConstantBuffer = true;

	using TAlignedType = TAlignedTypedef<glm::mat3x3, Alignment>::Type;
};

template<>
struct TShaderParameterTypeInfo<glm::fmat3x4>
{
	static constexpr EUniformBufferBaseType BaseType = UBMT_FLOAT32;
	static constexpr int32_t NumRows = 3;
	static constexpr int32_t NumColumns = 4;
	static constexpr int32_t NumElements = 0;
	static constexpr int32_t Alignment = 16;
	static constexpr bool bIsStoredInConstantBuffer = true;

	using TAlignedType = TAlignedTypedef<glm::mat3x4, Alignment>::Type;
};

template<>
struct TShaderParameterTypeInfo<glm::fmat4x4>
{
	static constexpr EUniformBufferBaseType BaseType = UBMT_FLOAT32;
	static constexpr int32_t NumRows = 4;
	static constexpr int32_t NumColumns = 4;
	static constexpr int32_t NumElements = 0;
	static constexpr int32_t Alignment = 16;
	static constexpr bool bIsStoredInConstantBuffer = true;

	using TAlignedType = TAlignedTypedef<glm::mat4x4, Alignment>::Type;
};

// todo(slavi) add some macros to handle layout processing

#endif // ENGINE_RENDERING_SHADERPARAMETERS_H
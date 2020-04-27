
#ifndef ENGINE_RHI_RHIRESOURCE_H
#define ENGINE_RHI_RHIRESOURCE_H

class RHIResource;
class RHIHardwareBuffer;
class RHIVertexBuffer;
class RHIIndexBuffer;
class RHIUniformBuffer;
class RHIStructuredBuffer;
class RHITexture;
class RHITexture2D;
class RHITexture3D;
typedef std::unique_ptr<RHIResource> Resource_uPtr;
typedef std::shared_ptr<RHIResource> Resource_ptr;
typedef std::weak_ptr<RHIResource> Resource_weakPtr;
typedef std::shared_ptr<RHIHardwareBuffer> RHIHardwareBuffer_ptr;
typedef std::shared_ptr<RHIVertexBuffer> RHIVertexBuffer_ptr;
typedef std::shared_ptr<RHIIndexBuffer> RHIIndexBuffer_ptr;
typedef std::shared_ptr<RHIUniformBuffer> RHIUniformBuffer_ptr;
typedef std::shared_ptr<RHIStructuredBuffer> RHIStructuredBuffer_ptr;
typedef std::shared_ptr<RHITexture> RHITexture_ptr;
typedef std::shared_ptr<RHITexture2D> RHITexture2D_ptr;
typedef std::shared_ptr<RHITexture3D> RHITexture3D_ptr;

enum RHIHardwareBufferUsage : uint8_t
{
	// we could specify general uses, such as STREAM_DRAW
	HWBU_STATIC = 1 << 0, // to be filled once
	HWBU_DYNAMIC = 1 << 2, // to be modified more often
	HWBU_DISCARD = 1 << 3, // to be streamed every frame

	// other uses with buffer data
	HWBU_WRITE_ONLY = 1 << 4, // contents are not read back
	HWBU_READ_ONLY = 1 << 5, // contents are to be read from the gpu, often RT usage

	HWBU_STATIC_WRITE_ONLY = HWBU_STATIC | HWBU_WRITE_ONLY, // optimal buffer usage
	HWBU_DYNAMIC_WRITE_ONLY = HWBU_DYNAMIC | HWBU_WRITE_ONLY, // write only dynamic

	HWBU_DYNAMIC_WRITE_ONLY_DISCARD = HWBU_DYNAMIC_WRITE_ONLY | HWBU_DISCARD, // dyanmic, write only discardable buffer for regular refilling
};

enum RHIIndexBufferType {
	IBT_16,
	IBT_32,
};

class RHIResource : std::enable_shared_from_this<RHIResource>
{
public:

protected:
	RHIResource() = default;

private:
};

class RHIHardwareBuffer : public RHIResource
{
public:
	RHIHardwareBuffer(RHIHardwareBufferUsage usage)
		: mUsage(usage), mSize(0) {}

	RHIHardwareBuffer(RHIHardwareBuffer&) = delete; // delete copy constructor

	OBJECT_GETACCESSOR(RHIHardwareBufferUsage, RHIHardwareBufferUsage, Usage);
	OBJECT_GETACCESSOR(size_t, size_t, Size);

protected:
	RHIHardwareBufferUsage mUsage;
	size_t mSize;
};

class RHIVertexBuffer : public RHIHardwareBuffer
{
public:
	RHIVertexBuffer(size_t verticesCount, size_t vertexSize, RHIHardwareBufferUsage usage)
		: RHIHardwareBuffer(usage), mVerticesCount(verticesCount), mVertexSize(vertexSize) {
		mSize = verticesCount * vertexSize;
	}

	OBJECT_GETACCESSOR(size_t, size_t, VerticesCount);
	OBJECT_GETACCESSOR(size_t, size_t, VertexSize);

private:
	size_t mVerticesCount;
	size_t mVertexSize;
};

class RHIIndexBuffer : public RHIHardwareBuffer
{
public:
	RHIIndexBuffer(size_t indicesCount, RHIHardwareBufferUsage usage, RHIIndexBufferType indexType = RHIIndexBufferType::IBT_32)
		: RHIHardwareBuffer(usage), mIndicesCount(indicesCount) {
		mIndexSize = indexType == RHIIndexBufferType::IBT_16
			? sizeof(uint16_t)
			: sizeof(uint32_t);
		mSize = indicesCount * mIndexSize;
	}

	OBJECT_GETACCESSOR(size_t, size_t, IndicesCount);
	OBJECT_GETACCESSOR(size_t, size_t, IndexSize);

private:
	size_t mIndicesCount;
	size_t mIndexSize;
};

class RHIUniformBuffer : public RHIHardwareBuffer
{
public:
private:
};

class RHIStructuredBuffer : public RHIHardwareBuffer
{
public:
private:
};

class RHITexture : public RHIResource
{
public:
	virtual RHITexture2D* GetTexture2D() { return nullptr; }
	virtual RHITexture3D* GetTexture3D() { return nullptr; }

private:
};

class RHITexture2D : public RHITexture
{
public:
	RHITexture2D(uint32_t width, uint32_t height)
		: RHITexture(), mWidth(width), mHeight(height) {}

	RHITexture2D* GetTexture2D() override final { return this; }

	inline glm::uvec2 GetSize() {
		return glm::fvec2(mWidth, mHeight);
	}

	inline uint32_t GetWidth() { return mWidth; }
	inline uint32_t GetHeight() { return mHeight; }

private:
	uint32_t mWidth;
	uint32_t mHeight;
};

class RHITexture3D : public RHITexture
{
public:
	RHITexture3D(uint32_t width, uint32_t height, uint32_t depth)
		: RHITexture(), mWidth(width), mHeight(height), mDepth(depth) {}

	RHITexture3D* GetTexture3D() override final { return this; }

	inline glm::uvec2 GetSize() {
		return glm::fvec2(mWidth, mHeight);
	}

	inline uint32_t GetWidth() { return mWidth; }
	inline uint32_t GetHeight() { return mHeight; }
	inline uint32_t GetDepth() { return mDepth; }

private:
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mDepth;
};


#endif // ENGINE_RHI_RHIRESOURCE_H

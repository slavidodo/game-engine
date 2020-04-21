
#ifndef FRAMEWORK_GRAPHICS_RENDERING_HARDWARE_BUFFER_H
#define FRAMEWORK_GRAPHICS_RENDERING_HARDWARE_BUFFER_H

namespace framework {
namespace graphics {
namespace rendering {

class HardwareBuffer : public std::enable_shared_from_this<HardwareBuffer>
{
public:
	enum Usage {
		// we could specify general uses, such as STREAM_DRAW
		USAGE_STATIC = 1 << 0, // to be filled once
		USAGE_DYNAMIC = 1 << 2, // to be modified more often
		USAGE_DISCARD = 1 << 3, // to be streamed every frame (look GL_STREAM)

		// other uses with buffer data
		USAGE_WRITE_ONLY = 1 << 4, // contents are not read back
		USAGE_READ_ONLY = 1 << 5, // contents are to be read from the gpu, often RT usage

		USAGE_STATIC_WRITE_ONLY = USAGE_STATIC | USAGE_WRITE_ONLY, // optimal buffer usage
		USAGE_DYNAMIC_WRITE_ONLY = USAGE_DYNAMIC | USAGE_WRITE_ONLY, // write only dynamic

		USAGE_DYNAMIC_WRITE_ONLY_DISCARD = USAGE_DYNAMIC_WRITE_ONLY | USAGE_DISCARD, // dyanmic, write only discardable buffer for regular refilling
	};

public:
	HardwareBuffer(Usage usage)
		: m_usage(usage) {}

	HardwareBuffer(HardwareBuffer&) = delete; // delete copy constructor

	OBJECT_GETACCESSOR(Usage, Usage, usage);
	OBJECT_GETACCESSOR(size_t, size_t, size);

	virtual void read(size_t size, size_t offset, void* dest) = 0;
	virtual void write(void* src, size_t size, size_t offset, bool discard) = 0;

protected:
	Usage m_usage;
	size_t m_size;
};

} // ns rendering
} // ns graphics
} // ns framework

#endif // FRAMEWORK_GRAPHICS_RENDERING_HARDWARE_BUFFER_H

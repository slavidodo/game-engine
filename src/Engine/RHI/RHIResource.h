
#ifndef ENGINE_RHI_RHIRESOURCE_H
#define ENGINE_RHI_RHIRESOURCE_H

class RHIResource;
typedef std::unique_ptr<RHIResource> Resource_uPtr;
typedef std::shared_ptr<RHIResource> Resource_ptr;
typedef std::weak_ptr<RHIResource> Resource_weakPtr;

class RHIResource : std::enable_shared_from_this<RHIResource>
{
public:

protected:
	RHIResource() = default;

private:

};

#endif // ENGINE_RHI_RHIRESOURCE_H

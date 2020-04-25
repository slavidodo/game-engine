
#ifndef ENGINE_RHI_RHITEXTURE_H
#define ENGINE_RHI_RHITEXTURE_H

#include "RHIResource.h"

// the actual render target used by the specified interface
// used generally to simulate frame buffers and things that
// should be done on the gpu, i.e particles
class RHITexture;
typedef std::shared_ptr<RHITexture> RHITexture_ptr;

class RHITexture : RHIResource
{
public:
private:
};

class RHITexture2D : public RHITexture
{
public:
private:
};

class RHITexture3D : public RHITexture
{
public:
private:
};

#endif // ENGINE_RHI_RHITEXTURE_H

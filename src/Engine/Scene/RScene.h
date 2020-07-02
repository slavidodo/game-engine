
#ifndef ENGINE_SCENE_SCENE_H
#define ENGINE_SCENE_SCENE_H

#include "SceneNode.h"
#include "RActor.h"
#include "../Core/Camera.h"
#include "../RHI/RHICommandList.h"
#include "Engine/Rendering/ShaderParameterMacros.h"

__declspec(align(SHADER_PARAMETER_STRUCT_ALIGNMENT))
class PrimitiveUniformShaderParameters {
public:
	PrimitiveUniformShaderParameters()
		: LocalToWorld(1.0f) {}

	TShaderParameterTypeInfo<glm::fmat4x4>::TAlignedType LocalToWorld;

	// TODO we don't wanna bother actually calculating the alignment for members
	// for simplicity and the existence of a single member only, we do it like that
	// but later on, we should calculate the layout and store it in a map identified
	// by the name of the parameters struct
	static RHIUniformBufferLayout& GetLayout() {
		static RHIUniformBufferLayout Layout;
		if (Layout.Resources.size() == 0) {
			// TODO(slavi) should be calculated automatically
			Layout.ConstantBufferSize = 16 * sizeof(float);
			Layout.Resources.push_back({0, UBMT_FLOAT32});
		}

		return Layout;
	}
};

class RScene;
typedef std::shared_ptr<RScene> RScene_ptr;

class RScene
{
public:
	/*Camera_ptr GetMainCamera() const {
		return mMainCamera;
	}*/

	std::function<void(RHICommandList& RHICmdList)> mRenderFunction;

	void AddActor(RActor_ptr actor);
	void RemoveActor(RActor_ptr actor);

	Camera_ptr mMainCamera;

	RHIVertexShaderRef mVertexShaderRHI;
	RHIPixelShaderRef mPixelShaderRHI;
	RHIBoundShaderStateRef mBoundShaderState;
	UniformBufferRef<PrimitiveUniformShaderParameters> mUniformBuffer;

protected:
	SceneNode_ptr mRootNode;

	std::list<RActor_ptr> mActors;
};

#endif // ENGINE_SCENE_SCENE_H
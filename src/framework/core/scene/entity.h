
#ifndef FRAMEWORK_CORE_SCENE_ENTITY_H
#define FRAMEWORK_CORE_SCENE_ENTITY_H

#include <framework/core/transform.h>
#include <framework/core/scene/object.h>

namespace framework {
namespace core {
namespace scene {

class Entity : public Object
{
public:
	OBJECT_ACCESSOR_DYNAMIC(glm::fvec3, glm::fvec3, position, m_transform.translation, m_transform.translation);

	Object_ptr attach(Object_ptr&& obj, std::string name);
	Object_ptr attach(Object* obj, std::string name);

protected:
	std::vector<Object_ptr> m_childObjects;
	std::map<std::string, int32_t> m_objectsByName;

	Transform m_transform;

private:
	Object_ptr attachInternal(Object_ptr&& obj, std::string name);
};

} // ns scene
} // ns core
} // ns framework

#endif
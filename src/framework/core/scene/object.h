
#ifndef FRAMEWORK_CORE_SCENE_OBJECT_H
#define FRAMEWORK_CORE_SCENE_OBJECT_H

namespace framework {
namespace core {
namespace scene {

class SceneNode;

class Object;
typedef std::shared_ptr<Object> Object_ptr;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object() {};

protected:
	std::weak_ptr<SceneNode> m_parent;
	friend class SceneNode;
};

} // ns scene
} // ns core
} // ns framework

#endif
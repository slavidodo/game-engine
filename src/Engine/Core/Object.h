
#ifndef ENGINE_CORE_OBJECT_H
#define ENGINE_CORE_OBJECT_H

class Object;
typedef std::shared_ptr<Object> Object_ptr;

class SceneNode;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object() {};

protected:
	std::weak_ptr<SceneNode> mParent;
	friend class SceneNode;
};

#endif // ENGINE_CORE_OBJECT_H
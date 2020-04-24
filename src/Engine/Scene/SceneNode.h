
#ifndef ENGINE_SCENE_SCENENODE_H
#define ENGINE_SCENE_SCENENODE_H

#include "../Core/Object.h"
#include "../Core/Transform.h"

class SceneNode;
typedef std::shared_ptr<SceneNode> SceneNode_ptr;
typedef std::shared_ptr<SceneNode> SceneNode_weakPtr;

class SceneManager;

// TODO; abstract this into a node class
/*
 * SceneNode represents the hierarchy of the scene elements
 * it breaks the whole scene down into several nodes bound by
 * a root node
 */
class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
    Object_ptr attachObject(Object_ptr&& obj);
    Object_ptr attachObject(Object* obj);

    OBJECT_GETACCESSOR(Transform_ptr, const Transform_ptr, transform);

    template<typename T>
    std::shared_ptr<T> getAttachedObject() const {
        auto it = m_attachedObjects.begin();
        auto end = m_attachedObjects.end();
        while (it != end) {
            std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(*it);
            if (p != nullptr) {
                return *p;
            }

            it++;
        }

        return nullptr;
    }

    template<>
    Transform_ptr getAttachedObject() const {
        return m_transform;
    }

private:
    Object_ptr attachObjectInternal(Object_ptr&& obj);

    std::vector<SceneNode_ptr> m_children;
    mutable std::vector<Object_ptr> m_attachedObjects;
	SceneNode_weakPtr m_parent;
    Transform_ptr m_transform;
    bool m_active;

protected:
    friend class SceneManager;
    SceneNode() = default;
};

#endif // ENGINE_SCENE_SCENENODE_H
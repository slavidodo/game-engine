
#ifndef ENGINE_SCENE_SCENENODE_H
#define ENGINE_SCENE_SCENENODE_H

#include "../Core/Object.h"
#include "../Core/Transform.h"

class SceneNode;
typedef std::shared_ptr<SceneNode> SceneNode_ptr;
typedef std::shared_ptr<SceneNode> SceneNode_weakPtr;

class SceneManager;
class Scene;

// TODO; abstract this into a node class
/*
 * SceneNode represents the hierarchy of the scene elements
 * it breaks the whole scene down into several nodes bound by
 * a root node
 */
class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
    Object_ptr AttachObject(Object_ptr&& obj);
    Object_ptr AttachObject(Object* obj);

    Transformtr GetTransform() const { return mTransform; }

    template<typename T>
    std::shared_ptr<T> GetAttachedObject() const {
        auto it = mAttachedObjects.begin();
        auto end = mAttachedObjects.end();
        while (it != end) {
            std::shared_ptr<T> p = std::static_pointer_cast<T>(*it);
            if (p != nullptr) {
                return *p;
            }

            it++;
        }

        return nullptr;
    }

    template<>
    Transformtr GetAttachedObject() const {
        return mTransform;
    }

private:
    Object_ptr AttachObjectInternal(Object_ptr&& obj);

    std::vector<SceneNode_ptr> mChildren;
    mutable std::vector<Object_ptr> mAttachedObjects;
	SceneNode_weakPtr mParent;
    Transformtr mTransform;
    bool mActive;

protected:
    friend class SceneManager;
    SceneNode() = default;
};

#endif // ENGINE_SCENE_SCENENODE_H
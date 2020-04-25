
#include "pch.h"

#include "SceneNode.h"

Object_ptr SceneNode::AttachObject(Object_ptr&& obj)
{
	return AttachObjectInternal(std::move(obj));
}

Object_ptr SceneNode::AttachObject(Object* obj)
{
	return AttachObjectInternal(std::move(std::shared_ptr<Object>(obj)));
}

Object_ptr SceneNode::AttachObjectInternal(Object_ptr&& obj)
{
	obj->mParent = weak_from_this();

	mAttachedObjects.push_back(std::move(obj));
	return mAttachedObjects.back();
}
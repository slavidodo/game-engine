
#include "pch.h"

#include "SceneNode.h"

Object_ptr SceneNode::attachObject(Object_ptr&& obj)
{
	return attachObjectInternal(std::move(obj));
}

Object_ptr SceneNode::attachObject(Object* obj)
{
	return attachObjectInternal(std::move(std::shared_ptr<Object>(obj)));
}

Object_ptr SceneNode::attachObjectInternal(Object_ptr&& obj)
{
	obj->mParent = weak_from_this();

	mAttachedObjects.push_back(std::move(obj));
	return mAttachedObjects.back();
}
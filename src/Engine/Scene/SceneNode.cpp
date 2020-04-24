
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
	obj->m_parent = weak_from_this();

	m_attachedObjects.push_back(std::move(obj));
	return m_attachedObjects.back();
}

#include <framework/pch.h>

#include "entity.h"

using namespace framework::core::scene;

Object_ptr Entity::attach(Object_ptr&& obj, std::string name)
{
	return attachInternal(std::move(obj), name);
}

Object_ptr Entity::attach(Object* obj, std::string name)
{
	return attachInternal(std::move(std::make_shared<Object>(obj)), name);
}

Object_ptr Entity::attachInternal(Object_ptr&& obj, std::string name)
{
	m_childObjects.push_back(std::move(obj));
	m_objectsByName[name] = m_childObjects.size() - 1;
	return m_childObjects.back();
}
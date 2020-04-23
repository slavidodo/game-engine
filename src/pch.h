
#ifndef FRAMEWORK_PCH_H
#define FRAMEWORK_PCH_H

#define NOMINMAX

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <ostream>
#include <array>
#include <tuple>

#define SDL_MAIN_HANDLED

#define OBJECT_GETACCESSOR(type, c_type, name) \
type name() { return m_##name; } \
c_type name() const { return m_##name; }

#define OBJECT_SETACCESSOR(type, name) void name(type name) { m_##name = name; }

#define OBJECT_GETACCESSOR_DYNAMIC(type, c_type, name, state, ...) \
type name() { return state(__VA_ARGS__); } \
c_type name() const { return state(__VA_ARGS__); }

#define OBJECT_SETACCESSOR_DYNAMIC(type, name, state) void name(type value) { state(value); }

#define OBJECT_ACCESSOR(type, c_type, name) \
OBJECT_SETACCESSOR(type, name) \
OBJECT_GETACCESSOR(type, c_type, name)

#define OBJECT_ACCESSOR_DYNAMIC(type, c_type, name, set_state, get_state, ...) \
OBJECT_SETACCESSOR_DYNAMIC(type, name, set_state) \
OBJECT_GETACCESSOR_DYNAMIC(type, c_type, name, get_state, __VA_ARGS__)

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif

#ifndef FRAMEWORK_PCH_H
#define FRAMEWORK_PCH_H

#define NOMINMAX

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <memory>
#include <ostream>
#include <array>
#include <tuple>
#include <functional>

#if __has_include("vulkan/vulkan.h")
#define GLFW_INCLUDE_VULKAN
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// todo get rid of these
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define OBJECT_GETACCESSOR(type, c_type, name) \
type Get##name() { return m##name; } \
c_type Get##name() const { return m##name; }

#define OBJECT_SETACCESSOR(type, name) void Set##name(type name) { m##name = name; }

#define OBJECT_GETACCESSOR_DYNAMIC(type, c_type, name, state, ...) \
type Get##name() { return state(__VA_ARGS__); } \
c_type Get##name() const { return state(__VA_ARGS__); }

#define OBJECT_SETACCESSOR_DYNAMIC(type, name, state) void Set##name(type value) { state(value); }

#define OBJECT_ACCESSOR(type, c_type, name) \
OBJECT_SETACCESSOR(type, name) \
OBJECT_GETACCESSOR(type, c_type, name)

#define OBJECT_ACCESSOR_DYNAMIC(type, c_type, name, set_state, get_state, ...) \
OBJECT_SETACCESSOR_DYNAMIC(type, name, set_state) \
OBJECT_GETACCESSOR_DYNAMIC(type, c_type, name, get_state, __VA_ARGS__)

#endif

#ifndef FRAMEWORK_PCH_H
#define FRAMEWORK_PCH_H

#include "Definitions.h"

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
#include <bitset>
#include <functional>
#include <atomic>
#include <mutex>
#include <thread>

#if __has_include("vulkan/vulkan.h")
#define GLFW_INCLUDE_VULKAN
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// todo get rid of these
#include <glm/glm.hpp>
#include <glm/ext.hpp>

template<typename T>
inline T Align(T Value, size_t Alignment)
{
	static_assert(std::is_integral<T>::value || std::is_pointer<T>::value, "Align expects an integer of pointer type.");
	return (T)(((size_t)Value + Alignment - 1) & ~(Alignment - 1));
}

#endif
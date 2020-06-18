
#ifndef FRAMEWORK_DEFINITIONS_H
#define FRAMEWORK_DEFINITIONS_H

/// definitions
#if defined(DEBUG) || !defined(NDEBUG) || defined(_DEBUG)
#define BUILD_DEBUG 1
#else
#define BUILD_DEBUG 0
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ARCH_64
#else
#define ARCH_32
#endif
#elif _WIN64
#define ARCH_64
#else
#define ARCH_32
#endif // 

#if _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#define WIN32_LEAN_AND_MEAN

#ifdef _MSC_VER
#pragma warning(disable:4244) // 'argument' : conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4267) // 'argument' : conversion from 'size_t' to 'type', possible loss of data
#endif // _MSC_VER

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602 // Windows 7
#endif // _WIN32_WINNT
#endif // _WIN32

#endif
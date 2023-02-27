#pragma once
#ifdef DEBUG_MODE
#if defined(PLATFORM_WINDOWS)
#define __DEBUGBREAK() __debugbreak()
#elif defined(PLATFORM_LINUX)
#include <signal.h>
#define __DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define __ENABLE_ASSERTS
#else
#define __DEBUGBREAK()
#endif
#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include <cassert>
#include <cstring>
#include <iostream>

#define ASSERT(x) assert(x)

#ifndef NDEBUG
#define LOG_DEBUG(component, msg) \
{                                                               \
    std::cout << "[" << #component << "] " << msg << std::endl; \
}
#else
#define LOG_DEBUG(component, msg)
#endif  // NDEBUG

#define LOG(component, msg) \
{                                                               \
    std::cout << "[" << #component << "] " << msg << std::endl; \
}

#define LOG_FATAL(component, msg) \
{                                                                       \
    std::cerr << "[" << #component << "] FATAL: " << msg << std::endl;  \
    std::abort();                                                       \
    __builtin_unreachable();                                            \
}

#define UNREACHABLE() \
{                                                                       \
    std::abort();                                                       \
    __builtin_unreachable();                                            \
}

#endif  // COMMON_MACRO_H

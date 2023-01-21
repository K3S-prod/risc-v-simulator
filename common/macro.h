#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include <cassert>
#include <cstring>
#include <iostream>

#define ASSERT(x) assert(x)

enum class LoggerComponent
{
    MMU,
    INTERPRETER,
};

static constexpr bool ComponentEnabled(LoggerComponent comp)
{
    switch (comp)
    {
        // case LoggerComponent::MMU:
        case LoggerComponent::INTERPRETER:
            return true;
        default:
            return false;
    }
}

#ifndef NDEBUG
#define LOG_DEBUG(component, msg) \
{                                                               \
    if (ComponentEnabled(LoggerComponent::component))           \
    std::cout << "[" << #component << "] " << msg << std::endl; \
}
#else
#define LOG_DEBUG(component, msg)
#endif  // NDEBUG

#define LOG(component, msg) \
{                                                               \
    std::cout << "[" << #component << "] " << msg << std::endl; \
}

#ifndef WIN32

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

#else // WIN32

#define LOG_FATAL(component, msg) \
{                                                                       \
    std::cerr << "[" << #component << "] FATAL: " << msg << std::endl;  \
    exit(EXIT_FAILURE);                                                 \
}

#define UNREACHABLE() \
{                                                                       \
    exit(EXIT_FAILURE);                                                 \
}

#endif // WIN32
#endif  // COMMON_MACRO_H

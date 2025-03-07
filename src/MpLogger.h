#ifndef MP_LOGGER_H
#define MP_LOGGER_H

#include "Log.h"

class MpLogger
{
public:
    template<typename... Args>
    static void debug(const char* fmt, Args&&... args) {
        LOG_DEBUG("module.MythicPlus", "[MythicPlus] DEBUG " + std::string(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(const char* fmt, Args&&... args) {
        LOG_ERROR("module.MythicPlus", "[MythicPlus] ERROR " + std::string(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(const char* fmt, Args&&... args) {
        LOG_INFO("module.MythicPlus", "[MythicPlus] INFO " + std::string(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(const char* fmt, Args&&... args) {
        LOG_WARN("module.MythicPlus", "[MythicPlus] WARN " + std::string(fmt), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void trace(const char* fmt, Args&&... args) {
        LOG_TRACE("module.MythicPlus", "[MythicPlus] TRACE " + std::string(fmt), std::forward<Args>(args)...);
    }
};


#endif // MP_LOGGER_H

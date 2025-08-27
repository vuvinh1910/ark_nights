#include "DebugLog.h"
#include <cstdarg>
#include <cstdio>

static std::vector<std::string> debugLogs;
static const int MAX_LOG_LINES = 50;

void AddDebugLog(const char* fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    debugLogs.push_back(std::string(buf));
    if (debugLogs.size() > MAX_LOG_LINES)
        debugLogs.erase(debugLogs.begin());
}

const std::vector<std::string>& GetDebugLogs() {
    return debugLogs;
}

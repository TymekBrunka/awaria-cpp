#include <Components_internal.hpp>

void* CompGlobals::icon_tex = nullptr;
std::map<tm, Day> CompGlobals::days{};
std::mutex CompGlobals::mutex{};

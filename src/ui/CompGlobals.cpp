#include <Components_internal.hpp>

void* CompGlobals::icon_tex = nullptr;
std::map<std::string, Day> CompGlobals::days{};
std::mutex CompGlobals::mutex{};

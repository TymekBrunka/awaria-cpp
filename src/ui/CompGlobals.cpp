#include <Components_internal.hpp>

void *CompGlobals::icon_tex = nullptr;
std::map<std::string, Day> CompGlobals::days{};
std::mutex CompGlobals::mutex{};
std::string CompGlobals::today_formated{};
std::chrono::year_month_day CompGlobals::today{};
std::string CompGlobals::file{};
std::string CompGlobals::current_selected_day{};

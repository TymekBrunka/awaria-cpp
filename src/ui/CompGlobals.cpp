#include <Components_internal.hpp>

void *CompGlobals::icon_tex = nullptr;
std::map<std::string, Day> CompGlobals::days{};
std::mutex CompGlobals::mutex{};
std::string CompGlobals::today_formated{};
std::chrono::year_month_day CompGlobals::today{};
std::string CompGlobals::file{};
std::string CompGlobals::current_selected_day{};
int CompGlobals::current_selected_template = -1;

std::chrono::year_month_day CompGlobals::start{};
std::chrono::year_month_day CompGlobals::end{};

tab CompGlobals::selected_tab = tab::DAYS;
std::vector<Template> CompGlobals::templates;

Template* CompGlobals::template_view = nullptr;

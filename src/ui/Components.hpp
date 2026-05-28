#pragma once
#include <chrono>
#include <dataclasses.hpp>
#include <map>
#include <mutex>

enum class tab {
  DAYS,
  TEMPLATES
};

struct CompGlobals {
  static void *icon_tex;
  static std::mutex mutex;
  static std::string file;
  static std::string today_formated;
  static std::chrono::year_month_day today;
  static std::map<std::string, Day> days;
  static std::string current_selected_day;
  static int current_selected_template;
  static tab selected_tab;

  static std::vector<Template> templates;
  static Template* template_view;

  static std::chrono::year_month_day start;
  static std::chrono::year_month_day end;
};

struct Menu {
  static void Ui();
};

struct SidePanel {
  static void Ui();
};

struct MainPanel {
  static void MainView();
};

struct TableRow {
  static bool Ui(Row &row);
};

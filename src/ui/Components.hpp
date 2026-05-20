#pragma once
#include <chrono>
#include <dataclasses.hpp>
#include <map>
#include <mutex>

struct CompGlobals {
  static std::mutex mutex;
  static std::string file;
  static std::string today_formated;
  static std::chrono::year_month_day today;
  static void *icon_tex;
  static std::map<std::string, Day> days;
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

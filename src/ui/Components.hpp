#pragma once
#include <dataclasses.hpp>
#include <mutex>
#include <map>

struct CompGlobals {
  static std::mutex mutex;
  static void *icon_tex;
  static std::map<tm, Day> days;
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
  static void Ui(const Row &row);
};

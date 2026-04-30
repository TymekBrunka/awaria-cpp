#pragma once
#include <dataclasses.hpp>

struct CompGlobals {
  static void *icon_tex;
  static std::vector<Day> days;
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

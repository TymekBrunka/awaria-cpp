#include "raylib.h"
#include "rlimgui.h"
#include "imgui.h"

int main(void)
{
  InitWindow(600, 800, "awaria");
  rlImGuiSetup(true);
  while (!WindowShouldClose()) {
    rlImGuiBegin();


    rlImGuiEnd();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

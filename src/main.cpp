#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "imgui_internal.h"

int main(void)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(800, 600, "awaria");
  SetTargetFPS(60);

  rlImGuiSetup(true);
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x222222ff));
    DrawFPS(10, 10);
    rlImGuiBegin();

    // ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::DockSpace(ImGui::GetID("Dockspace"));

    ImGui::Begin("hello");
    ImGui::Text("hai");
    ImGui::End();

    // ImGui::SetNextWindowPos(ImVec2(0,0));
    // ImGui::SetNextWindowSize(ImVec2(GetScreenWidth(), GetScreenHeight()));
    // ImGui::Begin("window", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    //
    // ImGui::BeginGroup();
    // ImGui::EndGroup();
    //
    // ImGui::Separator();
    //
    // ImGui::BeginGroup();
    // ImGui::EndGroup();
    //
    // ImGui::End();

    rlImGuiEnd();
    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

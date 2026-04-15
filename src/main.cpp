#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(800, 600, "awaria");
  SetTargetFPS(60);

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImFont *font1 = io.Fonts->AddFontFromFileTTF("src/Roboto-Regular.ttf", 30);

  ImGuiStyle &style = ImGui::GetStyle();
  style.FontSizeBase = 17.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x222222ff));

    rlImGuiBegin();
    ImGui::PushFont(font1, 0.0f);

    ImGuiID dockspace_id = ImGui::GetID("My Dockspace");
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGuiWindowClass window_class_horizontal;
    window_class_horizontal.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeY;

    ImGuiWindowClass window_class_vertical;
    window_class_vertical.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResizeX;

    // Create settings
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
      ImGuiID dock_id_menu = 0;
      ImGuiID dock_id_main = dockspace_id;
      ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Up, 30 * (1.0/viewport->Size.y), &dock_id_menu, &dock_id_main);
      ImGuiID dock_id_left = 0;
      ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.30f, &dock_id_left, &dock_id_main);
      ImGui::DockBuilderDockWindow("Menu", dock_id_menu);
      ImGui::DockBuilderDockWindow("Entries", dock_id_left);
      ImGui::DockBuilderDockWindow("Content", dock_id_main);
      ImGui::DockBuilderFinish(dockspace_id);
    }

    // Submit dockspace
    ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::SetNextWindowClass(&window_class_horizontal);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Button("Otwórz");
    ImGui::SameLine();
    ImGui::Button("Zapisz");
    ImGui::End();
    ImGui::PopStyleVar(1);

    ImGui::SetNextWindowClass(&window_class_horizontal);
    ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove);
    ImGui::End();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove);
    ImGui::End();

    ImGui::PopFont();
    rlImGuiEnd();

    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

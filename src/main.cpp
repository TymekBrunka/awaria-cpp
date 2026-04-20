#include "IconsFontAwesome6.h"
#include "extras/IconsFontAwesome6.h"
#include "fa.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"

char filter_buffer[500] = {0};

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(800, 600, "awaria");
  SetTargetFPS(60);

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImFontConfig fontcfg;
  // fontcfg.PixelSnapH = true;
  // fontcfg.FontDataOwnedByAtlas = false;
  fontcfg.MergeMode = true;

  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFont* font1 = io.Fonts->AddFontFromFileTTF("src/Roboto-Regular.ttf", 18);
  io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.0f, &fontcfg, icons_ranges);
  // ImFont* icons = io.Fonts->AddFontFromMemoryTTF(iconfont_data, iconfontsize, 16, &fontcfg);

  ImGuiStyle &style = ImGui::GetStyle();
  style.FontSizeBase = 17.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x222222ff));

    rlImGuiBegin();

    ImGui::PushFont(font1);
    ImGui::ShowStyleEditor();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0xff222222)); // ABGR

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
      ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Up, 30 * (1.0 / viewport->Size.y), &dock_id_menu, &dock_id_main);
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0);
    if (ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

      ImGui::SetCursorPos(ImVec2(3, 3));
      ImGui::Button(ICON_FA_ARROW_UP_FROM_BRACKET" Otwórz");
      ImGui::SameLine();
      ImGui::Button(ICON_FA_DOWNLOAD" Zapisz");
      ImGui::PopStyleVar(3);
    }
    ImGui::End();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
      ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

      ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
      ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
      ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (1 * window_padding.x) - item_spacing.x, 23);

      ImGui::Button(ICON_FA_SQUARE_PLUS" Dodaj", button_size);
      ImGui::SameLine();
      ImGui::Button(ICON_FA_SQUARE_MINUS" Usuń", button_size);
      ImGui::InputText("##Filtr", filter_buffer, 500);
      ImGui::PopStyleVar(1);
    }
    ImGui::End();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    if (ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove)) {
    }
    ImGui::End();
    ImGui::PopFont();

    ImGui::PopStyleColor(1);
    rlImGuiEnd();

    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

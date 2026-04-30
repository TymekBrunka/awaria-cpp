#include "IconsFontAwesome6.h"
#include "icon.png.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"
#include <iostream>

#include <Style.hpp>
#include <Components.hpp>

// union mychar {
//   uint32_t _int;
//   unsigned char _char[4];
// };
//
// mychar datetime_u32_buffer[17] = {0};
//
// #define utf8clen(x) 1 + (((x) & 0xC0) == 0xC0) + (((x) & 0xE0) == 0xE0) + (((x) & 0xF0) == 0xF0)
// #define utf8u32len(x) utf8clen((x)._char[0])
//
// int parse_dt_utf8(const char *buf, int bufDataLen) {
//   int utf8len = 0;
//   for (int i = 0; i < bufDataLen;) {
//     int charlen = utf8clen(buf[i]);
//     unsigned char *c = (unsigned char *)&buf[i];
//
//     datetime_u32_buffer[utf8len]._int = 0;
//     for (int j = 0; j < charlen; j++) {
//       datetime_u32_buffer[utf8len]._char[j] = *(c++);
//     }
//     i += charlen;
//     utf8len++;
//   }
//   return utf8len;
// }

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  // SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(800, 600, "awaria");

  Image icon = {
      .data = icon_data.data,
      .width = icon_data.width,
      .height = icon_data.height,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };
  SetWindowIcon(icon);

  Texture2D icon_tex = LoadTextureFromImage(icon);
  CompGlobals::icon_tex = &icon_tex;

  SetTargetFPS(60);

  rlImGuiSetup(true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImFontConfig fontcfg;
  // fontcfg.PixelSnapH = true;
  // fontcfg.FontDataOwnedByAtlas = false;
  fontcfg.MergeMode = true;

  static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImFont *font1 = io.Fonts->AddFontFromFileTTF("src/Roboto-Regular.ttf", 17);
  io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, 16.0f, &fontcfg, icons_ranges);
  // ImFont* icons = io.Fonts->AddFontFromMemoryTTF(iconfont_data, iconfontsize, 16, &fontcfg);

  ImGuiStyle &style = ImGui::GetStyle();
  style.FontSizeBase = 17.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x222222ff));

    rlImGuiBegin();

    ImGui::PushFont(font1);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(0xff222222)); // ABGR

    ImGui::ShowStyleEditor();
    ImGui::ShowDemoWindow();

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
    Menu::Ui();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    SidePanel::Ui();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    MainPanel::MainView();

    ImGui::PopFont();

    ImGui::PopStyleColor(1);
    rlImGuiEnd();

    EndDrawing();
  }
  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

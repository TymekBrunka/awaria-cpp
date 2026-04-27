#include "IconsFontAwesome6.h"
#include "icon.png.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"
#include <cstdint>
#include <iostream>
#include <stdio.h>

char filter_buffer[500] = {0};
char datetime_min_buffer[18] = {0};
char datetime_max_buffer[18] = {0};

struct StyleDelete {
  const static ImU32 button = 0xFF21218A;
  const static ImU32 text = 0xFF000039;
  const static ImU32 buttonhovered = 0xFF2B2BC6;
  const static ImU32 buttonactive = 0xFF3333E6;
};

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

// tak, to było na tyle dziwne że ściągnąłem tę funkcję z czatu
static int MaskedInputCallback(ImGuiInputTextCallbackData *data) {
  char digits[13] = {0}; // Tylko cyfry dla formatu 999999999999
  int dCount = 0;

  // 1. Wyciągnij same cyfry z tego, co jest w buforze
  for (int n = 0; n < data->BufTextLen && dCount < 13; n++) {
    if (data->Buf[n] >= '0' && data->Buf[n] <= '9') {
      digits[dCount++] = data->Buf[n];
    }
  }

  // 2. Zbuduj sformatowany ciąg: 0000:00:00 00:00
  char formatted[18];
  snprintf(formatted, 17, "%c%c%c%c:%c%c:%c%c %c%c:%c%c", dCount > 0 ? digits[0] : '0', dCount > 1 ? digits[1] : '0', dCount > 2 ? digits[2] : '0', dCount > 3 ? digits[3] : '0', dCount > 4 ? digits[4] : '0', dCount > 5 ? digits[5] : '0', dCount > 6 ? digits[6] : '0', dCount > 7 ? digits[7] : '0', dCount > 8 ? digits[8] : '0', dCount > 9 ? digits[9] : '0', dCount > 10 ? digits[10] : '0', dCount > 11 ? digits[11] : '0');

  // 3. Podmień zawartość bufora bez przesuwania kursora przez ImGui
  int cpos = data->CursorPos;
  data->DeleteChars(0, data->BufTextLen);
  data->InsertChars(0, formatted);
  data->CursorPos = cpos;

  // Opcjonalnie: utrzymuj kursor na końcu cyfr lub w stałym miejscu
  return 0;
}

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
      ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0xff222222));
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(0xffCCCCCC));

      ImGui::SetCursorPos(ImVec2(3, 3));
      rlImGuiImageSize(&icon_tex, 20, 20);
      ImGui::SameLine();
      ImGui::Button(ICON_FA_ARROW_UP_FROM_BRACKET " Otwórz");
      ImGui::SameLine();
      ImGui::Button(ICON_FA_DOWNLOAD " Zapisz");
      ImGui::PopStyleVar(3);
      ImGui::PopStyleColor(2);
    }
    ImGui::End();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
      // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

      ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
      ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
      ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

      ImGui::Button(ICON_FA_SQUARE_PLUS " Dodaj", button_size);
      ImGui::SameLine();
      ImGui::Button(ICON_FA_SQUARE_MINUS " Usuń", button_size);

      ImGui::TextAligned(0.5, ImGui::GetWindowSize().x - (0.5 * window_padding.x), "Wpisy");

      ImGui::BeginChild("Hai");
      for (int i = 0; i < 2000; i++) {
        ImGui::PushID(i);
        ImGui::Button("2026-12-24", ImVec2(ImGui::GetWindowSize().x - (0.5 * window_padding.x) - 45, 23));

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, StyleDelete::button);
        ImGui::PushStyleColor(ImGuiCol_Text, StyleDelete::text);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StyleDelete::buttonhovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, StyleDelete::buttonactive);
        ImGui::Button(ICON_FA_TRASH);
        ImGui::PopStyleColor(4);
        ImGui::PopID();
      }
      ImGui::EndChild();

      // ImGui::PopStyleVar(1);
    }
    ImGui::End();

    ImGui::SetNextWindowClass(&window_class_horizontal);
    if (ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove)) {
      ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
      ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
      ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x - item_spacing.x, 23);

      ImGui::TextUnformatted("Filtr");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - (2 * window_padding.x) - item_spacing.x - ImGui::CalcTextSize("Filtr").x);
      ImGui::InputText("##Filtr", filter_buffer, 500);

      ImGui::TextUnformatted("od");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize("od").x);
      ImGui::InputText("##datetimi_min", datetime_min_buffer, 18, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback);

      ImGui::SameLine();
      ImGui::TextUnformatted("do");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize("do").x);
      ImGui::InputText("##datetimi_max", datetime_max_buffer, 18, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback);

      ImGui::BeginChild("Przefiltrowane");
      // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
      for (int i = 0; i < 2000; i++) {
        ImGui::PushID(i);
        ImGui::Text("Dzien #%d", i);
        for (int j = 0; j < 5; j++) {
          ImGui::PushID(j);
          bool bul = false;
          ImGui::Checkbox("##hehe", &bul);
          ImGui::SameLine();

          ImGui::Button("zadanie");
          ImGui::SameLine();

          ImGui::Button(ICON_FA_PEN);
          ImGui::SameLine();

          ImGui::PushStyleColor(ImGuiCol_Button, StyleDelete::button);
          ImGui::PushStyleColor(ImGuiCol_Text, StyleDelete::text);
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StyleDelete::buttonhovered);
          ImGui::PushStyleColor(ImGuiCol_ButtonActive, StyleDelete::buttonactive);
          ImGui::Button(ICON_FA_TRASH);
          ImGui::PopStyleColor(4);

          ImGui::PushStyleColor(ImGuiCol_FrameBg, ImU32(0xff111111));
          ImGui::BeginTable("tabelka", 2);
          ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed);
          ImGui::TableNextRow();

          ImGui::TableSetColumnIndex(0);
          ImGui::TextUnformatted("Sitopiastownik");
          ImGui::TableSetColumnIndex(1);
          ImGui::TextUnformatted("Zamki z piasku");
          ImGui::TableNextRow();

          ImGui::TableSetColumnIndex(0);
          ImGui::TextUnformatted("Osadnik");
          ImGui::TableSetColumnIndex(1);
          ImGui::TextUnformatted("Osadzanie się");

          ImGui::EndTable();
          ImGui::PopStyleColor(1);

          ImGui::PopID();
        }
        ImGui::PopID();
        ImGui::TextUnformatted("");
      }
      // ImGui::PopStyleVar(1);
      ImGui::EndChild();
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

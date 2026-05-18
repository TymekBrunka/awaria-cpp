#include "Components.hpp"
#include "IconsFontAwesome6.h"
#include "Style.hpp"
// #include "imgui.h"
#include <Components_internal.hpp>
#include <stdio.h>

static char filter_buffer[500] = {0};

static char datetime_min_buffer[18 * 2] = {0}; // double size to fit in another detetime string when pasting
static char datetime_max_buffer[18 * 2] = {0}; // double size to fit in another detetime string when pasting

static int datetime_min_length = 0;
static int datetime_max_length = 0;

static int MaskedInputCallback(ImGuiInputTextCallbackData *data) {
  char digits[13] = {0}; // Tylko cyfry dla formatu 999999999999
  int dCount = 0;
  int new_len = data->BufTextLen;

  // 1. Wyciągnij same cyfry z tego, co jest w buforze
  for (int n = 0; n < data->BufTextLen && dCount < 13; n++) {
    if (data->Buf[n] >= '0' && data->Buf[n] <= '9') {
      digits[dCount++] = data->Buf[n];
    }
  }

  // 2. Zbuduj sformatowany ciąg: 0000:00:00 00:00
  char formatted[18];
  snprintf(formatted, 17, "%c%c%c%c-%c%c-%c%c %c%c:%c%c", dCount > 0 ? digits[0] : '0', dCount > 1 ? digits[1] : '0', dCount > 2 ? digits[2] : '0', dCount > 3 ? digits[3] : '0', dCount > 4 ? digits[4] : '0', dCount > 5 ? digits[5] : '0', dCount > 6 ? digits[6] : '0', dCount > 7 ? digits[7] : '0', dCount > 8 ? digits[8] : '0', dCount > 9 ? digits[9] : '0', dCount > 10 ? digits[10] : '0', dCount > 11 ? digits[11] : '0');

  int cpos = data->CursorPos;
  data->DeleteChars(0, data->BufTextLen);
  data->InsertChars(0, formatted);

  int &prev_length = *(int *&)data->UserData;
  int diff = cpos - prev_length;

  // if (diff > 0)
  //   for (int i = 0; i < new_len; i++) {
  //     if (prev_length + i == 5 || prev_length + i == 7 || prev_length + i == 11 || prev_length + i == 13) {
  //       prev_length++;
  //       cpos++;
  //     }
  //   }
  if ((cpos == 5 || cpos == 7 || cpos == 11 || cpos == 13) && diff > 0)
    cpos++;

  data->CursorPos = cpos;
  prev_length = cpos;
  return 0;
}

void MainPanel::MainView() {
  if (ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove)) {
    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x - item_spacing.x, 23);

    ImGui::TextUnformatted(ICON_FA_MAGNIFYING_GLASS " Filtr");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - (2 * window_padding.x) - item_spacing.x - ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS " Filtr").x);
    ImGui::InputText("##Filtr", filter_buffer, 500);

    ImGui::TextUnformatted(ICON_FA_CALENDAR_DAYS " od");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize(ICON_FA_CALENDAR_DAYS " od").x);
    ImGui::InputText("##datetimi_min", datetime_min_buffer, 18 * 2, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback, &datetime_min_length);

    ImGui::SameLine();
    ImGui::TextUnformatted("do");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize("do").x);
    ImGui::InputText("##datetimi_max", datetime_max_buffer, 18 * 2, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback, &datetime_max_length);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::BeginChild("Przefiltrowane");
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    // auto day = CompGlobals::days.begin();
    // for (int i = 0; i < CompGlobals::days.size(); i++) {
    // for (int i = 0; i < 1000; i++) {
    int i = 0;
    for (auto& [date, day] : CompGlobals::days) {
      ImGui::PushID(i);

      StyleDelete::Button(ICON_FA_CALENDAR_MINUS);
      ImGui::SameLine();
      ImGui::Button(ICON_FA_HAMMER "+ dodaj zadanie");
      ImGui::SameLine();

      ImGui::Text("Dzień %s", date.data());
      ImGui::Dummy(ImVec2(0.0f, 3.0f));

      ImGui::Indent(8.0f);

      for (int j = day.shift1.tasks.size(); j > 0; j--) {
        bool important = false;
        if (important) {
          ImGui::PushStyleColor(ImGuiCol_FrameBg, StyleImportant::framebg);
          ImGui::PushStyleColor(ImGuiCol_Button, StyleImportant::framebg);
        }

        ImGui::PushID(j);
        bool bul = false;
        ImGui::Checkbox("##hehe", &bul);
        ImGui::SameLine();

        StyleDelete::Button(ICON_FA_HAMMER " --");
        ImGui::SameLine();
        ImGui::Button(ICON_FA_TRIANGLE_EXCLAMATION "+");
        ImGui::SameLine();
        bool switch_important = ImGui::Button(ICON_FA_THUMBTACK);
        ImGui::SameLine();
        ImGui::InputText("##zadanie", day.shift1.tasks[j].description.data(), 10);

        if (important)
          ImGui::PopStyleColor(2);

        ImGui::BeginTable("Uwagi", 4 /*, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg*/);
        if (important)
          ImGui::PushStyleColor(ImGuiCol_FrameBg, StyleImportant::dimmed_framebg);
        else
          ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF151515);

        ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("col2", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("col3", ImGuiTableColumnFlags_WidthFixed);

        Row rows[] = {{"Igły", "", "Zamki z piasku"}, {"Igły", "Osadnik", "Osad"}, {"", "Rzeka", "Woda jest za zimna na kąpiel"}};

        ImGui::Indent(8.0f);
        for (int k = 0; k < (sizeof(rows) / sizeof(Row)); k++) {
          ImGui::PushID(k);
          ImGui::TableNextRow();
          TableRow::Ui(rows[k]);
          ImGui::PopID();
        }
        ImGui::Unindent(8.0f);
        ImGui::PopStyleColor(1);
        ImGui::EndTable();

        ImGui::PopID();

        if (switch_important)
          important = !important;
      }
      ImGui::PopID();

      ImGui::Unindent(8.0f);
      ImGui::TextUnformatted("Awarie");

      ImGui::BeginTable("Awarie", 4 /*, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg*/);
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF151515);

      ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("col2", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("col3", ImGuiTableColumnFlags_WidthFixed);

      Row rows[] = {{"Igły", "Sitopiastownik", "Zamki z piasku i nie tylko"}, {"Igły", "Osadnik", "Osadzanie się węgla brunatnego"}};

      for (int k = 0; k < (sizeof(rows) / sizeof(Row)); k++) {
        ImGui::PushID(k);
        ImGui::TableNextRow();
        TableRow::Ui(rows[k]);
        ImGui::PopID();
      }

      ImGui::PopStyleColor(1);
      ImGui::EndTable();

      ImGui::Dummy(ImVec2(0.0f, 20.0f));
      ++i;
    }
    // ImGui::PopStyleVar(1);
    ImGui::EndChild();
  }
  ImGui::End();
}

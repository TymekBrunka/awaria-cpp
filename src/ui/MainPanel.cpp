#include "Components.hpp"
#include "IconsFontAwesome6.h"
#include "Style.hpp"
#include "imgui.h"
#include <Components_internal.hpp>
#include <imgui_stdlib.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>

char datetime_min_buffer[12 * 2] = {0}; // double size to fit in another detetime string when pasting
char datetime_max_buffer[12 * 2] = {0}; // double size to fit in another detetime string when pasting

struct cb_data {
  int &prev_len;
  std::chrono::year_month_day &ymd;
};

static int datetime_min_length = 0;
static int datetime_max_length = 0;

static int MaskedInputCallback(ImGuiInputTextCallbackData *data) {
  char digits[13] = {0}; // Tylko cyfry dla formatu 999999999999
  int dCount = 0;
  int new_len = data->BufTextLen;

  cb_data *cbdata = (cb_data *)data->UserData;
  std::chrono::year_month_day &ymd = cbdata->ymd;

  if (!new_len) {
    if (&ymd == &CompGlobals::start) {
      ymd = std::chrono::year_month_day(std::chrono::year(1970), std::chrono::month(01), std::chrono::day(01));
      memcpy(datetime_min_buffer, "1970-01-01", 13);
      data->DeleteChars(0, data->BufTextLen);
      data->InsertChars(0, datetime_min_buffer);
      data->CursorPos = 0;
    } else {
      ymd = CompGlobals::today;
      memcpy(datetime_max_buffer, CompGlobals::today_formated.data(), 13);
      data->DeleteChars(0, data->BufTextLen);
      data->InsertChars(0, datetime_max_buffer);
      data->CursorPos = 0;
    }
    return 0;
  }

  // 1. Wyciągnij same cyfry z tego, co jest w buforze
  for (int n = 0; n < data->BufTextLen && dCount < 13; n++) {
    if (data->Buf[n] >= '0' && data->Buf[n] <= '9') {
      digits[dCount++] = data->Buf[n];
    }
  }

  // 2. Zbuduj sformatowany ciąg: 0000:00:00 00:00
  char formatted[14];
  snprintf(formatted, 13, "%c%c%c%c-%c%c-%c%c", dCount > 0 ? digits[0] : '0', dCount > 1 ? digits[1] : '0', dCount > 2 ? digits[2] : '0', dCount > 3 ? digits[3] : '0', dCount > 4 ? digits[4] : '0', dCount > 5 ? digits[5] : '0', dCount > 6 ? digits[6] : '0', dCount > 7 ? digits[7] : '0');

  int cpos = data->CursorPos;
  data->DeleteChars(0, data->BufTextLen);
  data->InsertChars(0, formatted);

  int &prev_length = cbdata->prev_len;
  int diff = cpos - prev_length;

  // if (diff > 0)
  //   for (int i = 0; i < new_len; i++) {
  //     if (prev_length + i == 5 || prev_length + i == 7 || prev_length + i == 11 || prev_length + i == 13) {
  //       prev_length++;
  //       cpos++;
  //     }
  //   }
  if ((cpos == 5 || cpos == 7) && diff > 0)
    cpos++;

  data->CursorPos = cpos;
  prev_length = cpos;

  std::tm time{};
  std::istringstream ss(data->Buf);
  ss >> std::get_time(&time, "%Y-%m-%d");

  if (ss.fail())
    return 0;

  ymd = std::chrono::year_month_day(std::chrono::year(time.tm_year + 1900), std::chrono::month(time.tm_mon + 1), std::chrono::day(time.tm_mday));
  return 0;
}

static void task_list(Shift &shift) {
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0xff151515));
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(0xffCCCCCC));
  if (ImGui::Button(ICON_FA_HAMMER "+ dodaj zadanie")) {
    shift.tasks.push_back({});
  }
  ImGui::PopStyleColor(2);

  ImGui::Indent(8.0f);

  for (int j = shift.tasks.size() - 1; j >= 0; j--) {
    bool &important = shift.tasks[j].important;
    if (important) {
      ImGui::PushStyleColor(ImGuiCol_FrameBg, StyleImportant::framebg);
      ImGui::PushStyleColor(ImGuiCol_Button, StyleImportant::framebg);
    }

    ImGui::PushID(j);
    ImGui::BeginDisabled(shift.tasks[j].notices.size());
    ImGui::Checkbox("##hehe", &shift.tasks[j].finished);
    ImGui::EndDisabled();
    ImGui::SameLine();

    if (StyleDelete::Button(ICON_FA_HAMMER " --")) {
      ImGui::PopID();
      if (important) {
        ImGui::PopStyleColor(2);
      }
      shift.tasks.erase(shift.tasks.begin() + j);
      break;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_TRIANGLE_EXCLAMATION "+")) {
      shift.tasks[j].notices.push_back({});
    }
    ImGui::SameLine();
    bool switch_important = ImGui::Button(ICON_FA_THUMBTACK);
    ImGui::SameLine();
    ImGui::InputText("##zadanie", &shift.tasks[j].description);

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

    int k = 0;
    ImGui::Indent(8.0f);
    for (auto &notice : shift.tasks[j].notices) {
      ImGui::PushID(k);
      ImGui::TableNextRow();
      if (TableRow::Ui(notice)) {
        shift.tasks[j].notices.erase(shift.tasks[j].notices.begin() + k);
        ImGui::PopID();
        break;
      }
      ImGui::PopID();
      ++k;
    }
    ImGui::Unindent(8.0f);
    ImGui::PopStyleColor(1);
    ImGui::EndTable();

    ImGui::PopID();

    if (switch_important)
      important = !important;
  }
  ImGui::Unindent(8.0f);
}

void template_task_list(Shift &shift) {
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0xff151515));
  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(0xffCCCCCC));
  if (ImGui::Button(ICON_FA_HAMMER "+ dodaj zadanie")) {
    shift.tasks.push_back({});
  }
  ImGui::PopStyleColor(2);

  for (int j = shift.tasks.size() - 1; j >= 0; j--) {
    ImGui::PushID(j);

    bool &important = shift.tasks[j].important;
    if (important) {
      ImGui::PushStyleColor(ImGuiCol_FrameBg, StyleImportant::framebg);
      ImGui::PushStyleColor(ImGuiCol_Button, StyleImportant::framebg);
    }
    if (StyleDelete::Button(ICON_FA_HAMMER " --")) {
      ImGui::PopID();
      if (important) {
        ImGui::PopStyleColor(2);
      }
      shift.tasks.erase(shift.tasks.begin() + j);
      break;
    }
    ImGui::SameLine();
    bool switch_important = ImGui::Button(ICON_FA_THUMBTACK);
    ImGui::SameLine();
    ImGui::InputText("##zadanie", (std::string *)&shift.tasks[j].description);

    if (important)
      ImGui::PopStyleColor(2);

    if (switch_important)
      important = !important;

    ImGui::PopID();
  }
}

void MainPanel::MainView() {
  if (CompGlobals::selected_tab == tab::DAYS) {
    if (ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove)) {
      ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
      ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
      ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x - item_spacing.x, 23);

      // ImGui::TextUnformatted(ICON_FA_MAGNIFYING_GLASS " Filtr");
      // ImGui::SameLine();
      // ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - (2 * window_padding.x) - item_spacing.x - ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS " Filtr").x);
      // ImGui::InputText("##Filtr", filter_buffer, 500);

      ImGui::TextUnformatted(ICON_FA_CALENDAR_DAYS " od");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize(ICON_FA_CALENDAR_DAYS " od").x);
      cb_data min_{datetime_min_length, CompGlobals::start};
      ImGui::InputText("##datetimi_min", datetime_min_buffer, 18 * 2, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback, &min_);

      ImGui::SameLine();
      ImGui::TextUnformatted("do");
      ImGui::SameLine();
      ImGui::SetNextItemWidth(button_size.x - ImGui::CalcTextSize("do").x);
      cb_data max_{datetime_max_length, CompGlobals::end};
      ImGui::InputText("##datetimi_max", datetime_max_buffer, 18 * 2, ImGuiInputTextFlags_CallbackEdit, MaskedInputCallback, &max_);
      ImGui::Dummy(ImVec2(0.0f, 10.0f));

      ImGui::BeginChild("Przefiltrowane");
      // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

      // auto day = CompGlobals::days.begin();
      // for (int i = 0; i < CompGlobals::days.size(); i++) {
      // for (int i = 0; i < 1000; i++) {
      int i = 0;
      for (auto &[date, day] : CompGlobals::days) {
        if (day.ymd < CompGlobals::start || day.ymd > CompGlobals::end)
          continue;

        ImGui::PushID(i);

        if (StyleDelete::Button(ICON_FA_CALENDAR_MINUS)) {
          auto idx = CompGlobals::days.find(date);
          CompGlobals::days.erase(idx);
          CompGlobals::current_selected_day = "";
          ImGui::PopID();
          break;
        }
        ImGui::SameLine();
        ImGui::Text("Dzień %s", date.data());
        ImGui::Dummy(ImVec2(0.0f, 3.0f));

        ImGui::PushID(0);
        ImGui::TextUnformatted("Zmiana 1");
        task_list(day.shift1);
        ImGui::PopID();

        ImGui::PushID(1);
        ImGui::TextUnformatted("Zmiana 2");
        task_list(day.shift2);
        ImGui::PopID();

        ImGui::TextUnformatted("Awarie");
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0xff151515));
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(0xffCCCCCC));
        if (ImGui::Button(ICON_FA_TRIANGLE_EXCLAMATION "+ dodaj awarię")) {
          day.malfunctions.push_back({});
        }
        ImGui::PopStyleColor(2);

        ImGui::BeginTable("Awarie", 4 /*, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg*/);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF151515);

        ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("col2", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("col3", ImGuiTableColumnFlags_WidthFixed);

        int k = 0;
        for (auto &malfunction : day.malfunctions) {
          ImGui::PushID(k);
          ImGui::TableNextRow();
          if (TableRow::Ui(malfunction)) {
            day.malfunctions.erase(day.malfunctions.begin() + k);
            ImGui::PopID();
            break;
          }
          ImGui::PopID();
          ++k;
        }

        ImGui::PopStyleColor(1);
        ImGui::EndTable();
        ImGui::PopID();

        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ++i;
      }
      // ImGui::PopStyleVar(1);
      ImGui::EndChild();
    }
    ImGui::End();

  } else if (CompGlobals::selected_tab == tab::TEMPLATES) {

    if (ImGui::Begin("Content", NULL, ImGuiWindowFlags_NoMove)) {
      int i = 0;
      if (CompGlobals::current_selected_template != -1) {
        Template &szablon = CompGlobals::templates[CompGlobals::current_selected_template];
        do {
          ImGui::PushID(i);

          if (StyleDelete::Button(ICON_FA_TRASH)) {
            CompGlobals::templates.erase(CompGlobals::templates.begin() + i);
            CompGlobals::current_selected_template = -1;
            ImGui::PopID();
            break;
          }

          ImGui::SameLine();
          ImGui::Text("Szablon");
          ImGui::SameLine();
          ImGui::InputText("##nazwa", (std::string *)&szablon.name);
          ImGui::Dummy(ImVec2(0.0f, 3.0f));

          ImGui::PushID(0);
          ImGui::TextUnformatted("Zmiana 1");
          template_task_list(szablon.day.shift1);
          ImGui::PopID();

          ImGui::PushID(1);
          ImGui::TextUnformatted("Zmiana 2");
          template_task_list(szablon.day.shift2);
          ImGui::PopID();

          ImGui::PopID();
          ImGui::Dummy(ImVec2(0.0f, 20.0f));
          ++i;
        } while (0);
      }
    }
    ImGui::End();
  }
}

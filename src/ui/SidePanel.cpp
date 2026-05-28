#include <Components_internal.hpp>
#include <SDL3/SDL_dialog.h>
#include <cstring>
#include <imgui_stdlib.h>

extern char datetime_min_buffer[12 * 2];
extern char datetime_max_buffer[12 * 2];

static std::string template_name;

void loadDataFromFile();

void SidePanel::Ui() {
  if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    if (ImGui::BeginTabBar("MyTabBar")) {
      if (ImGui::BeginTabItem(ICON_FA_CALENDAR " Dnie")) {
        CompGlobals::selected_tab = tab::DAYS;
        ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
        ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
        ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

        // ImGui::Button(ICON_FA_CALENDAR " Dni", button_size);
        // ImGui::SameLine();
        // ImGui::Button(ICON_FA_SHEET_PLASTIC " Szablony", button_size);
        // // ImGui::Dummy(ImVec2(0.0f, 0.0f));
        // ImGui::Separator();
        // ImGui::Dummy(ImVec2(0.0f, 2.0f));

        ImGui::Dummy(ImVec2(0.0f, 0.0f));
        if (ImGui::Button(ICON_FA_CALENDAR_PLUS " Dodaj", button_size)) {
          CompGlobals::days[CompGlobals::today_formated] = {.ymd = CompGlobals::today};
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_CALENDAR_MINUS " Usuń", button_size)) {
          auto idx = CompGlobals::days.find(CompGlobals::current_selected_day);
          if (idx != CompGlobals::days.end()) {
            CompGlobals::days.erase(idx);
            CompGlobals::current_selected_day = "";
          }
        }

        // ImGui::TextAligned(0.5, ImGui::GetWindowSize().x - (0.5 * window_padding.x), "Wpisy");
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("wpisy").x) / 2.0);
        ImGui::TextUnformatted("wpisy");

        ImGui::BeginChild("Wpisy");
        int i = 0;
        bool is_selected = false;
        for (auto &[date, day] : CompGlobals::days) {
          ImGui::PushID(i);
          is_selected = date == CompGlobals::current_selected_day;
          if (is_selected)
            ImGui::PushStyleColor(ImGuiCol_Button, StyleImportant::framebg);

          if (ImGui::Button(date.data(), ImVec2(ImGui::GetWindowSize().x - (0.5 * window_padding.x) - 45, 23))) {
            CompGlobals::current_selected_day = date;
            CompGlobals::start = day.ymd;
            CompGlobals::end = day.ymd;
            memcpy(datetime_min_buffer, date.data(), 13);
            memcpy(datetime_max_buffer, date.data(), 13);
          }

          if (is_selected)
            ImGui::PopStyleColor(1);

          ImGui::SameLine();
          if (StyleDelete::Button(ICON_FA_TRASH)) {
            auto idx = CompGlobals::days.find(date);
            CompGlobals::days.erase(idx);
            CompGlobals::current_selected_day = "";
            ImGui::PopID();
            break;
          }
          ImGui::PopID();
          ++i;
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(ICON_FA_CUBE " Szablony")) {
        CompGlobals::selected_tab = tab::TEMPLATES;
        ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
        ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
        ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

        ImGui::Dummy(ImVec2(0.0f, 0.0f));
        if (ImGui::Button(ICON_FA_CUBE " Dodaj", button_size)) {
          CompGlobals::templates.push_back({.name = template_name, .day = {.ymd = CompGlobals::today}});
          if (CompGlobals::current_selected_template != -1)
            CompGlobals::template_view = &CompGlobals::templates[CompGlobals::current_selected_template];
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_RECTANGLE_XMARK " Usuń", button_size)) {
          if (CompGlobals::current_selected_template != -1 && CompGlobals::current_selected_template < CompGlobals::templates.size()) {
            CompGlobals::templates.erase(CompGlobals::templates.begin() + CompGlobals::current_selected_template);
            CompGlobals::current_selected_template = -1;
          }
        }
        ImGui::InputText("nazwa", &template_name);

        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("szablony").x) / 2.0);
        ImGui::TextUnformatted("szablony");

        ImGui::BeginChild("Szablony");
        int i = 0;
        for (auto& szablon : CompGlobals::templates) {
          ImGui::PushID(i);

          bool is_selected = i == CompGlobals::current_selected_template;
          if (is_selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, StyleImportant::framebg);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, StyleImportant::framebg);
          }

          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
          if (ImGui::Button(ICON_FA_CUBE, ImVec2(23, 22))) {
            CompGlobals::current_selected_template = i;
            CompGlobals::template_view = &szablon;
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - (0.5 * window_padding.x) - 23 - 45);
          ImGui::PopStyleVar(1);
          ImGui::InputText("##nazwa", (std::string *)&szablon.name);

          if (is_selected)
            ImGui::PopStyleColor(2);

          ImGui::SameLine();
          if (StyleDelete::Button(ICON_FA_TRASH)) {
            CompGlobals::templates.erase(CompGlobals::templates.begin() + i);
            CompGlobals::current_selected_template = -1;
            ImGui::PopID();
            break;
          }

          ImGui::PopID();
          ++i;
        }

        ImGui::EndChild();
        ImGui::EndTabItem();
      }
    }
    ImGui::EndTabBar();

    // ImGui::PopStyleVar(1);
  }
  ImGui::End();
}

#include <Components_internal.hpp>
#include <SDL3/SDL_dialog.h>
#include <cstring>

extern char datetime_min_buffer[12 * 2];
extern char datetime_max_buffer[12 * 2];

void loadDataFromFile();

void SidePanel::Ui() {
  if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

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

    // ImGui::PopStyleVar(1);
  }
  ImGui::End();
}

#include <Components_internal.hpp>
#include <SDL3/SDL_dialog.h>

void loadDataFromFile();

void SidePanel::Ui() {
  if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

    if (ImGui::Button(ICON_FA_CALENDAR_PLUS " Dodaj", button_size)) {
      CompGlobals::days[CompGlobals::today_formated];
    }
    ImGui::SameLine();
    ImGui::Button(ICON_FA_CALENDAR_MINUS " Usuń", button_size);

    // ImGui::TextAligned(0.5, ImGui::GetWindowSize().x - (0.5 * window_padding.x), "Wpisy");
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("wpisy").x) / 2.0);
    ImGui::TextUnformatted("wpisy");

    ImGui::BeginChild("Wpisy");
    int i = 0;
    for (auto& [date, day] : CompGlobals::days) {
      ImGui::PushID(i);
      ImGui::Button(date.data(), ImVec2(ImGui::GetWindowSize().x - (0.5 * window_padding.x) - 45, 23));

      ImGui::SameLine();
      if (StyleDelete::Button(ICON_FA_TRASH)) {
        auto idx = CompGlobals::days.find(date);
        CompGlobals::days.erase(idx);
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

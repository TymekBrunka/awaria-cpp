#include <Components_internal.hpp>

void SidePanel::Ui() {
  if (ImGui::Begin("Entries", NULL, ImGuiWindowFlags_NoMove)) {
    // ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 button_size = ImVec2(ImGui::GetWindowSize().x * 0.5f - (0.5 * window_padding.x) - item_spacing.x, 23);

    ImGui::Button(ICON_FA_SQUARE_PLUS " Dodaj", button_size);
    ImGui::SameLine();
    ImGui::Button(ICON_FA_SQUARE_MINUS " Usuń", button_size);

    // ImGui::TextAligned(0.5, ImGui::GetWindowSize().x - (0.5 * window_padding.x), "Wpisy");
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("wpisy").x) / 2.0);
    ImGui::TextUnformatted("wpisy");

    ImGui::BeginChild("Wpisy");
    for (int i = 0; i < 1000; i++) {
      ImGui::PushID(i);
      ImGui::Button("2026-12-24", ImVec2(ImGui::GetWindowSize().x - (0.5 * window_padding.x) - 45, 23));

      ImGui::SameLine();
      StyleDelete::PushStyleVars();
      ImGui::Button(ICON_FA_TRASH);
      StyleDelete::PopStyleColors();
      ImGui::PopID();
    }
    ImGui::EndChild();

    // ImGui::PopStyleVar(1);
  }
  ImGui::End();
}

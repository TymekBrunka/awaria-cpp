#include <Components_internal.hpp>
#include <rlImGui.h>

void Menu::Ui() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  // ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0);
  if (ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(0xff222222));
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(0xffCCCCCC));

    ImGui::SetCursorPos(ImVec2(3, 3));
    rlImGuiImageSize((const Texture*)CompGlobals::icon_tex, 20, 20);
    ImGui::SameLine();
    ImGui::Button(ICON_FA_ARROW_UP_FROM_BRACKET " Otwórz");
    ImGui::SameLine();
    ImGui::Button(ICON_FA_DOWNLOAD " Zapisz");
    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(2);
  }
  ImGui::End();
}

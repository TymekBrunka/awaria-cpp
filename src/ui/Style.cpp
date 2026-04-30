#include <Style.hpp>

void StyleDelete::PushStyleVars() {
  ImGui::PushStyleColor(ImGuiCol_Button, StyleDelete::button);
  ImGui::PushStyleColor(ImGuiCol_Text, StyleDelete::text);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StyleDelete::buttonhovered);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, StyleDelete::buttonactive);
}

void StyleDelete::PopStyleColors() { ImGui::PopStyleColor(4); }

bool StyleDelete::Button(const char* label) {
  ImGui::PushStyleColor(ImGuiCol_Button, StyleDelete::button);
  ImGui::PushStyleColor(ImGuiCol_Text, StyleDelete::text);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, StyleDelete::buttonhovered);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, StyleDelete::buttonactive);
  bool ret = ImGui::Button(label);
  ImGui::PopStyleColor(4);

  return ret;
}

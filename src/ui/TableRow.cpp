#include <Components_internal.hpp>
#include <imgui_stdlib.h>

bool TableRow::Ui(Row &row) {
  ImGui::TableSetColumnIndex(0);
  bool bul = false;
  if (StyleDelete::Button(ICON_FA_TRASH)) {
    bul = true;
  }
  ImGui::TableSetColumnIndex(1);
  ImGui::SetNextItemWidth(70);
  ImGui::InputText("##city", &row.city);
  ImGui::TableSetColumnIndex(2);
  ImGui::SetNextItemWidth(150);
  ImGui::InputText("##site", &row.site);
  ImGui::TableSetColumnIndex(3);
  ImGui::InputText("##description", &row.description);
  return bul;
}

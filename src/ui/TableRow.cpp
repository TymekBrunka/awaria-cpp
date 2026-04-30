#include <Components_internal.hpp>

void TableRow::Ui(const Row &row) {
  ImGui::TableSetColumnIndex(0);
  StyleDelete::Button(ICON_FA_TRASH);
  ImGui::TableSetColumnIndex(1);
  ImGui::SetNextItemWidth(70);
  ImGui::InputText("##city", (char*)row.city.data(), row.city.capacity());
  ImGui::TableSetColumnIndex(2);
  ImGui::SetNextItemWidth(150);
  ImGui::InputText("##site", (char*)row.site.data(), row.site.capacity());
  ImGui::TableSetColumnIndex(3);
  ImGui::InputText("##description", (char*)row.description.data(), row.description.capacity());
}

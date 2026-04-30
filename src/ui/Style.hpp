#pragma once
#include <imgui.h>

struct StyleDelete {
  const static ImU32 button = 0xFF21218A;
  const static ImU32 text = 0xFF000039;
  const static ImU32 buttonhovered = 0xFF2B2BC6;
  const static ImU32 buttonactive = 0xFF3333E6;

  static void PushStyleVars();
  static void PopStyleColors();
  static bool Button(const char *label);
};

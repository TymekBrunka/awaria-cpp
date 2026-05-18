#include "Components.hpp"
#include "SDL3/SDL_dialog.h"
#include <Components_internal.hpp>
#include <SDL3/SDL.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <rlImGui.h>
#include <sstream>
#include <string>
#include <yyjson.h>

static const SDL_DialogFileFilter ofd_filters[] = {{"harmonogram (json)", "harm.json;json"}, {"Wszystkie pliki", "*"}};

static void SDLCALL load_data_callback(void *userdata, const char *const *filelist, int filter);

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
    rlImGuiImageSize((const Texture *)CompGlobals::icon_tex, 20, 20);
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ARROW_UP_FROM_BRACKET " Otwórz")) {
      SDL_ShowOpenFileDialog(load_data_callback, nullptr, nullptr, ofd_filters, 2, NULL, false);
    }
    ImGui::SameLine();
    ImGui::Button(ICON_FA_DOWNLOAD " Zapisz");
    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(2);
  }
  ImGui::End();
}

std::string readFile(const char *filepath) {
  if (!std::filesystem::exists(filepath)) {
    throw std::string("Nie można otworzyć pliku: \"") + filepath + "\"\n";
  }

  std::ifstream inFile;
  inFile.open(filepath); // open the input file

  std::stringstream strStream;
  strStream << inFile.rdbuf();       // read the file
  std::string str = strStream.str(); // str holds the content of the file
  return str;
}

#define assume(x, msg)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
  if (!(x)) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd wczytywania pliku", (msg), NULL);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    std::cout << (msg) << "\n";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    yyjson_doc_free(doc);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
    return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           \
  }

#define assume2(x, msg)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  if (!(x)) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd wczytywania pliku", (msg), NULL);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    std::cout << (msg) << "\n";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    yyjson_doc_free(doc);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
    return 1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
  }

#define iter_obj(obj, iter, key)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
  yyjson_obj_iter iter;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  yyjson_val *key;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
  yyjson_obj_iter_init(obj, &iter);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
  while ((key = yyjson_obj_iter_next(&iter)))

#define iter_arr(arr, iter, idx, max)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 \
  size_t idx, max;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
  yyjson_val *iter;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
  yyjson_arr_foreach(arr, idx, max, iter)

static int load_shift(Shift &shift, yyjson_val *zmiana, yyjson_doc *doc) {
  iter_arr(zmiana, zadanie, zadanie_idx, zadanie_max) {
    if (yyjson_is_str(zadanie)) {
      shift.tasks.push_back({.description = yyjson_get_str(zadanie)});
    } else if (yyjson_is_obj(zadanie)) {
      yyjson_val *opis = yyjson_obj_get(zadanie, "opis");
      assume2(yyjson_is_str(opis), "pole (zadanie)opis nie jest ciągiem znaków");

      Task task{.description = yyjson_get_str(opis)};
      yyjson_val *wykonano = yyjson_obj_get(zadanie, "wykonano");
      yyjson_val *wazne = yyjson_obj_get(zadanie, "ważne");
      yyjson_val *uwagi = yyjson_obj_get(zadanie, "uwagi");

      if (wykonano) {
        assume2(yyjson_is_bool(wykonano), "pole (zadanie)wykonano nie jest prawdą/fałszem");
        task.finished = yyjson_get_bool(wykonano);
      }

      if (wazne) {
        assume2(yyjson_is_bool(wazne), "pole (zadanie)ważne nie jest prawdą/fałszem");
        task.important = yyjson_get_bool(wazne);
      }

      if (uwagi) {
        assume2(yyjson_is_arr(uwagi), "pole (zadanie)uwagi nie jest listą");
        iter_arr(uwagi, uwaga, uwaga_idx, uwaga_max) {
          assume2(yyjson_is_obj(uwaga), "element w (zadanie)liście uwag nie jest objektem");
          yyjson_val *opis = yyjson_obj_get(zadanie, "opis");
          assume2(yyjson_is_str(opis), "pole (uwaga)opis nie jest ciągiem znaków");

          Notice notice{.description = yyjson_get_str(opis)};
          yyjson_val *miejscowosc = yyjson_obj_get(zadanie, "miejscowość");
          yyjson_val *objekt = yyjson_obj_get(zadanie, "objekt");

          if (miejscowosc) {
            assume2(yyjson_is_str(miejscowosc), "pole (uwaga)miejscowość nie jest ciągiem znaków");
            notice.city = yyjson_get_str(miejscowosc);
          }

          if (objekt) {
            assume2(yyjson_is_str(objekt), "pole (uwaga)objekt nie jest ciągiem znaków");
            notice.site = yyjson_get_str(objekt);
          }

          task.notices.push_back(notice);
        }
      }

      shift.tasks.push_back(task);
    } else {
      assume2(false, "element w (wpis)liście zadań nie jest odpowiedniego typu (wspierane: opis(ciąg znaków), objekt)");
    }
  }
}

static void SDLCALL load_data_callback(void *userdata, const char *const *filelist, int filter) {
  if (!filelist) {
    return;
  } else if (!*filelist) {
    return;
  }

  std::string data;
  try {
    data = std::move(readFile(*filelist));
  } catch (std::string &str) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd otwarcia pliku", str.c_str(), NULL);
    return;
  }

  yyjson_doc *doc = yyjson_read(data.c_str(), data.size(), 0);
  // assume(doc, "Błąd wczytania pliku ...");
  yyjson_val *root = yyjson_doc_get_root(doc);

  yyjson_val *wersja_formatu = yyjson_obj_get(root, "wersja_formatu");
  assume(yyjson_is_num(wersja_formatu), "pole wersja_formatu nie jest liczbą");
  assume((double)yyjson_get_num(wersja_formatu) == 0, "Nieobsługiwana wersja formatu");

  yyjson_val *wpisy = yyjson_obj_get(root, "wpisy");
  assume(yyjson_is_obj(wpisy), "pole wpisy nie jest objektem");

  static std::map<std::string, Day> days;

  iter_obj(wpisy, wpis_, date_) {
    yyjson_val *wpis = yyjson_obj_iter_get_val(date_);
    const char *date = yyjson_get_str(date_);
    Day day{};

    yyjson_val *zmiana1 = yyjson_obj_get(wpis, "zmiana1");
    assume(yyjson_is_arr(zmiana1), "pole (wpis)zmiana1 nie jest listą");
    if (load_shift(day.shift1, zmiana1, doc)) return;

    yyjson_val *zmiana2 = yyjson_obj_get(wpis, "zmiana1");
    assume(yyjson_is_arr(zmiana2), "pole (wpis)zmiana1 nie jest listą");
    if (load_shift(day.shift2, zmiana2, doc)) return;

    days[date] = std::move(day);
  }
  yyjson_doc_free(doc);
  std::lock_guard<std::mutex> guard(CompGlobals::mutex);
  CompGlobals::days = std::move(days);
}

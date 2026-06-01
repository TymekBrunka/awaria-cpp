#include "Components.hpp"
#include "SDL3/SDL_dialog.h"
#include "dataclasses.hpp"
#include "imgui.h"
#include <Components_internal.hpp>
#include <SDL3/SDL.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <rlImGui.h>
#include <sstream>
#include <string>
#include <yyjson.h>

static const SDL_DialogFileFilter ofd_filters[] = {{"harmonogram (json)", "harm.json;json"}, {"Wszystkie pliki", "*"}};

static void SDLCALL load_data_callback(void *userdata, const char *const *filelist, int filter);
static void SDLCALL save_data_callback(void *userdata, const char *const *filelist, int filter);
static int save_file(std::string &file);

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
    if (ImGui::Button(ICON_FA_DOWNLOAD " Zapisz jako")) {
      SDL_ShowSaveFileDialog(save_data_callback, nullptr, nullptr, ofd_filters, 2, NULL);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_DOWNLOAD " Zapisz")) {
      save_file(CompGlobals::file);
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_INFO " o programie")) {
      ImGui::OpenPopup("about");
    }
    if (ImGui::BeginPopup("about")) {
      ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) - 20);
      rlImGuiImageSize((const Texture *)CompGlobals::icon_tex, 40, 40);
      ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Awaria wersja 0.9").x) / 2);
      ImGui::TextUnformatted("Awaria wersja 0.9");
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Awaria to harmonogram z opcjami dodawania uwag co do możliwości wykonania zadania");
      ImGui::TextUnformatted("oraz dodawania awarii, które wystąpiły w następującym dniu");
      ImGui::TextUnformatted("Autor: Tymek Brunka");
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Wykorzystane technologie:");
      ImGui::TextUnformatted("- Raylib (od raysan5 na githubie)");
      ImGui::TextUnformatted("- Dear ImGui (od ocornut na githubie)");
      ImGui::TextUnformatted("- rlImGui (z raylib-extras na githubie)");
      ImGui::TextUnformatted("- SDL(3) (z libsdl-org na githubie)");
      ImGui::TextUnformatted("- yyjson (od ibireme na githubie)");
      ImGui::EndPopup();
    }
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

#define assume3(x, msg)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
  if (!(x)) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd wczytywania pliku", (msg), NULL);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    std::cout << (msg) << "\n";                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    yyjson_mut_doc_free(doc);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         \
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
      yyjson_val *wykonane = yyjson_obj_get(zadanie, "wykonane");
      yyjson_val *wazne = yyjson_obj_get(zadanie, "ważne");
      yyjson_val *uwagi = yyjson_obj_get(zadanie, "uwagi");

      if (wykonane) {
        assume2(yyjson_is_bool(wykonane), "pole (zadanie)wykonane nie jest prawdą/fałszem");
        task.finished = yyjson_get_bool(wykonane);
      }

      if (wazne) {
        assume2(yyjson_is_bool(wazne), "pole (zadanie)ważne nie jest prawdą/fałszem");
        task.important = yyjson_get_bool(wazne);
      }

      if (uwagi) {
        assume2(yyjson_is_arr(uwagi), "pole (zadanie)uwagi nie jest listą");
        iter_arr(uwagi, uwaga, uwaga_idx, uwaga_max) {
          assume2(yyjson_is_obj(uwaga), "element w (zadanie)liście uwag nie jest objektem");
          yyjson_val *opis_ = yyjson_obj_get(uwaga, "opis");
          assume2(yyjson_is_str(opis_), "pole (uwaga)opis nie jest ciągiem znaków");

          Notice notice{.description = yyjson_get_str(opis_)};
          yyjson_val *miejscowosc = yyjson_obj_get(uwaga, "miejscowość");
          yyjson_val *objekt = yyjson_obj_get(uwaga, "objekt");

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
  return 0;
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
    CompGlobals::file = std::string(*filelist);
  } catch (std::string &str) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Błąd otwarcia pliku", str.c_str(), NULL);
    return;
  }

  yyjson_doc *doc = yyjson_read(data.c_str(), data.size(), YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS);
  // assume(!doc, "Błąd wczytania pliku ...");
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

    std::tm time{};
    std::istringstream ss(date);
    ss >> std::get_time(&time, "%Y-%m-%d");

    if (ss.fail()) {
      yyjson_doc_free(doc);
      std::stringstream ss;
      ss << "Błąd odczytu: nie udało się przetworzyć ciągu znaków na datę\n";
      assume(true, ss.str().c_str());
    }

    std::chrono::year_month_day ymd(std::chrono::year(time.tm_year + 1900), std::chrono::month(time.tm_mon + 1), std::chrono::day(time.tm_mday));
    day.ymd = ymd;

    yyjson_val *zmiana1 = yyjson_obj_get(wpis, "zmiana1");
    assume(yyjson_is_arr(zmiana1), "pole (wpis)zmiana1 nie jest listą");
    if (load_shift(day.shift1, zmiana1, doc))
      return;

    yyjson_val *zmiana2 = yyjson_obj_get(wpis, "zmiana2");
    assume(yyjson_is_arr(zmiana2), "pole (wpis)zmiana1 nie jest listą");
    if (load_shift(day.shift2, zmiana2, doc))
      return;

    yyjson_val *awarie = yyjson_obj_get(wpis, "awarie");
    if (awarie) {
      assume(yyjson_is_arr(awarie), "pole (wpis)awarie nie jest listą");
      iter_arr(awarie, awaria, awaria_idx, awaria_max) {
        assume(yyjson_is_obj(awaria), "element w liście awarie nie jest objektem");
        yyjson_val *opis_ = yyjson_obj_get(awaria, "opis");
        assume(yyjson_is_str(opis_), "pole (awaria)opis nie jest ciągiem znaków");

        Malfunction malfunction{.description = yyjson_get_str(opis_)};
        yyjson_val *miejscowosc = yyjson_obj_get(awaria, "miejscowość");
        yyjson_val *objekt = yyjson_obj_get(awaria, "objekt");

        if (miejscowosc) {
          assume(yyjson_is_str(miejscowosc), "pole (awaria)miejscowość nie jest ciągiem znaków");
          malfunction.city = yyjson_get_str(miejscowosc);
        }

        if (objekt) {
          assume(yyjson_is_str(objekt), "pole (awaria)objekt nie jest ciągiem znaków");
          malfunction.site = yyjson_get_str(objekt);
        }

        day.malfunctions.push_back(malfunction);
      }
    }

    days[date] = std::move(day);
  }

  std::vector<Template> templates;

  yyjson_val *szablony = yyjson_obj_get(root, "szablony");
  assume(yyjson_is_arr(szablony), "pole szablony nie jest objektem");
  iter_arr(szablony, szablon, szablon_idx, szablon_max) {
    assume(yyjson_is_obj(szablon), "element w liście szablonów nie jest objektem");
    yyjson_val *nazwa = yyjson_obj_get(szablon, "nazwa");
    assume(yyjson_is_str(nazwa), "pole (szablon)nazwa nie jest ciągiem znaków");

    Template template_{.name = yyjson_get_str(nazwa), .day = {.ymd = CompGlobals::today}};

    yyjson_val *zmiana1 = yyjson_obj_get(szablon, "zmiana1");
    assume(yyjson_is_arr(zmiana1), "pole (szablon)zmiana1 nie jest listą");
    if (load_shift(template_.day.shift1, zmiana1, doc))
      return;

    yyjson_val *zmiana2 = yyjson_obj_get(szablon, "zmiana2");
    assume(yyjson_is_arr(zmiana2), "pole (szablon)zmiana1 nie jest listą");
    if (load_shift(template_.day.shift2, zmiana2, doc))
      return;

    templates.push_back(template_);
  }

  yyjson_doc_free(doc);
  std::lock_guard<std::mutex> guard(CompGlobals::mutex);
  CompGlobals::current_selected_day = "";
  CompGlobals::current_selected_template = -1;
  CompGlobals::days = std::move(days);
  CompGlobals::templates = templates;
  CompGlobals::file = *filelist;
}

static int save_shift(yyjson_mut_doc *doc, yyjson_mut_val *zmiana1, Shift &shift) {
  for (auto &task : shift.tasks) {
    if (task.notices.size() == 0 && !task.finished && !task.important) {
      yyjson_mut_arr_add_str(doc, zmiana1, task.description.c_str());
    } else {
      yyjson_mut_val *zadanie = yyjson_mut_arr_add_obj(doc, zmiana1);

      // if (!task.description.empty()) {
      yyjson_mut_obj_add_str(doc, zadanie, "opis", task.description.c_str());
      // }
      if (task.important) {
        yyjson_mut_obj_add_bool(doc, zadanie, "ważne", true);
      }
      if (task.finished) {
        yyjson_mut_obj_add_bool(doc, zadanie, "wykonane", true);
      }
      if (task.notices.size() > 0) {
        yyjson_mut_val *uwagi = yyjson_mut_obj_add_arr(doc, zadanie, "uwagi");
        for (auto &notice : task.notices) {
          yyjson_mut_val *uwaga = yyjson_mut_arr_add_obj(doc, uwagi);
          yyjson_mut_obj_add_str(doc, uwaga, "miejscowość", notice.city.c_str());
          yyjson_mut_obj_add_str(doc, uwaga, "objekt", notice.site.c_str());
          yyjson_mut_obj_add_str(doc, uwaga, "opis", notice.description.c_str());
        }
      }
    }
  }
  return 0;
}

static int save_file(std::string &file) {
  yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
  yyjson_mut_val *root = yyjson_mut_obj(doc);
  yyjson_mut_doc_set_root(doc, root);
  yyjson_mut_obj_add_sint(doc, root, "wersja_formatu", 0);

  yyjson_mut_val *wpisy = yyjson_mut_obj_add_obj(doc, root, "wpisy");

  // std::lock_guard<std::mutex> guard(CompGlobals::mutex); //for some reason it was deadlocking (but i only use the mutex in 3 places (which are on single thread most of the time))
  for (auto &[date, day] : CompGlobals::days) {
    yyjson_mut_val *wpis = yyjson_mut_obj_add_obj(doc, wpisy, date.c_str());

    yyjson_mut_val *zmiana1 = yyjson_mut_obj_add_arr(doc, wpis, "zmiana1");
    yyjson_mut_val *zmiana2 = yyjson_mut_obj_add_arr(doc, wpis, "zmiana2");
    save_shift(doc, zmiana1, day.shift1);
    save_shift(doc, zmiana2, day.shift2);

    yyjson_mut_val *awarie = yyjson_mut_obj_add_arr(doc, wpis, "awarie");
    for (auto &malfunction : day.malfunctions) {
      yyjson_mut_val *awaria = yyjson_mut_arr_add_obj(doc, awarie);
      yyjson_mut_obj_add_str(doc, awaria, "miejscowość", malfunction.city.c_str());
      yyjson_mut_obj_add_str(doc, awaria, "objekt", malfunction.site.c_str());
      yyjson_mut_obj_add_str(doc, awaria, "opis", malfunction.description.c_str());
    }
  }

  yyjson_mut_val *szablony = yyjson_mut_obj_add_arr(doc, root, "szablony");
  for (auto &template_ : CompGlobals::templates) {
    yyjson_mut_val *szablon = yyjson_mut_arr_add_obj(doc, szablony);
    yyjson_mut_obj_add_str(doc, szablon, "nazwa", template_.name.c_str());

    yyjson_mut_val *zmiana1 = yyjson_mut_obj_add_arr(doc, szablon, "zmiana1");
    yyjson_mut_val *zmiana2 = yyjson_mut_obj_add_arr(doc, szablon, "zmiana2");
    save_shift(doc, zmiana1, template_.day.shift1);
    save_shift(doc, zmiana2, template_.day.shift2);
  }

  yyjson_write_err err;
  yyjson_mut_write_file(file.c_str(), doc, YYJSON_WRITE_PRETTY_TWO_SPACES, NULL, &err);
  if (err.code) {
    std::stringstream ss;
    ss << "Błąd zapisu: " << err.code << " " << err.msg << "\n";
    assume3(false, ss.str().c_str());
  }

  yyjson_mut_doc_free(doc);
  return 0;
}

static void SDLCALL save_data_callback(void *userdata, const char *const *filelist, int filter) {
  if (!filelist) {
    return;
  } else if (!*filelist) {
    return;
  }

  std::string f(*filelist);
  save_file(f);
}

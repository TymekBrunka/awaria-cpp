#pragma once
#include <string>
#include <vector>
#include <cstdint>

typedef std::string City;
typedef std::string Site;

struct Row {
  City city;
  Site site;
  std::string description;
};

typedef Row Notice;
typedef Row Malfunction;

struct Task {
  bool finished = false;
  bool important = false;
  std::vector<Notice> notices;
  std::string description;
};

struct Shift {
  std::vector<Task> tasks;
  std::vector<Malfunction> malfunctions;
};

struct Day {
  // const std::string formated_date;
  Shift shift1;
  Shift shift2;
};

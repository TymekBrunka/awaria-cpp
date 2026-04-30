#pragma once
#include <string>
#include <vector>
#include <ctime>
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
  bool finished;
  bool important;
  std::vector<Notice> notices;
  std::string description;
};

struct Shift {
  std::vector<Task> tasks;
  std::vector<Malfunction> malfunctions;
};

struct Day {
  tm date;
  Shift shift1;
  Shift shift2;
};

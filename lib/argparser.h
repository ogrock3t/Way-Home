#pragma once

#include <string>

struct TArgs {
  std::string date;
  std::string FromCity;
  std::string ToCity;
};

bool IsDigit(const char &symbol);
bool CheckDateFormat(const std::string &date);
bool ReadArgs(TArgs *args, int argc, char **argv);
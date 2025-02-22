#include "argparser.h"

#include <iostream>

bool IsDigit(const char &symbol) {
  if (symbol >= '0' && symbol <= '9')
    return true;
  return false;
}

bool CheckDateFormat(const std::string &date) {
  if (date.length() != 10)
    return false;

  if (date[4] != '-' || date[7] != '-')
    return false;

  for (size_t i = 0; i < 10; ++i) {
    if (i == 4 || i == 7)
      continue;

    if (!IsDigit(date[i]))
      return false;
  }

  int year = std::stoi(date.substr(0, 4));
  int month = std::stoi(date.substr(5, 2));
  int day = std::stoi(date.substr(8, 2));

  if (year != 2025)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;

  return true;
}

bool ReadArgs(TArgs *args, int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Ошибка: Недостаточно количество аргументов. Должно быть 3 "
                 "аргумента: <from_city> <to_city> <date (YYYY-MM-DD)>\n";
    return false;
  }

  args->FromCity = argv[1];
  args->ToCity = argv[2];
  args->date = argv[3];

  if (args->FromCity == args->ToCity) {
    std::cerr << "Ошибка: Город отправления и город назначения не должны "
                 "совпадать.\n";
    return false;
  }

  if (!CheckDateFormat(args->date)) {
    std::cerr << "Ошибка: Неверный формат даты. Используйте YYYY-MM-DD.\n";
    return false;
  }

  return true;
}
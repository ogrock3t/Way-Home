#include <iostream>

#include "lib/argparser.h"
#include "lib/cache.h"
#include "lib/routes.h"

int main(int argc, char **argv) {
  TArgs args;

  if (!ReadArgs(&args, argc, argv))
    return 1;

  std::string FromCode;
  std::string ToCode;
  std::string ApiKey = GetApiKey("apikey.json");

  if (args.FromCity == "Уфа") {
    FromCode = "c172";
    ToCode = "c2";
  } else {
    FromCode = "c2";
    ToCode = "c172";
  }

  std::cout << "Откуда: " << args.FromCity << '\n';
  std::cout << "Куда: " << args.ToCity << '\n';
  std::cout << "Дата: " << args.date << '\n';

  Get(FromCode, ToCode, ApiKey, args.date);

  return 0;
}
#include "routes.h"
#include "cache.h"

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

struct Segment {
  std::string depature;
  std::string arrival;
  std::string from;
  std::string to;
  std::string carrier;
  std::string url_carrier;
  std::string transport_type;
};

using json = nlohmann::json;

json MakeRequest(const std::string &FromCode, const std::string &ToCode,
                 const std::string &ApiKey, const std::string &date,
                 const std::string &transport_type) {
  std::string cache_key =
      GenerateCacheKey(FromCode, ToCode, date, transport_type);

  if (IsCacheValid(cache_key)) {
    return ReadCache(cache_key);
  } else {

    std::string url = "https://api.rasp.yandex.net/v3.0/search/?";
    url += "apikey=" + ApiKey;
    url += "&from=" + FromCode;
    url += "&to=" + ToCode;
    url += "&date=" + date;
    url += "&transfers=false";
    url += "&transport_types=" + transport_type;
    url += "&limit=10";

    cpr::Response response = cpr::Get(cpr::Url(url));

    if (response.status_code == 200) {
      json json_data = json::parse(response.text);

      WriteCache(cache_key, json_data);

      return json_data;
    } else {
      std::cerr << "Ошибка: при запросе: " << response.status_code << " ("
                << transport_type << ")\n";
      return nullptr;
    }
  }
}

std::vector<Segment> ParseSegments(const json &json_data,
                                   const std::string &transport_type) {
  std::vector<Segment> segments;
  for (const json &s : json_data["segments"]) {
    Segment seg;

    seg.depature = s.contains("departure") ? s["departure"].get<std::string>()
                                           : "Неизвестно";
    seg.arrival =
        s.contains("arrival") ? s["arrival"].get<std::string>() : "Неизвестно";
    seg.from = s.contains("from") && s["from"].contains("title")
                   ? s["from"]["title"].get<std::string>()
                   : "Неизвестно";
    seg.to = s.contains("to") && s["to"].contains("title")
                 ? s["to"]["title"].get<std::string>()
                 : "Неизвестно";
    seg.carrier = s.contains("thread") && s["thread"].contains("carrier") &&
                          s["thread"]["carrier"].contains("title")
                      ? s["thread"]["carrier"]["title"].get<std::string>()
                      : "Неизвестный перевозчик";
    seg.url_carrier = s.contains("thread") && s["thread"].contains("carrier") &&
                              s["thread"]["carrier"].contains("url")
                          ? s["thread"]["carrier"]["url"].get<std::string>()
                          : "";
    seg.transport_type = transport_type;

    seg.depature[10] = ' ';
    seg.arrival[10] = ' ';

    segments.push_back(seg);
  }

  return segments;
}

void Get(const std::string &FromCode, const std::string &ToCode,
         const std::string &ApiKey, const std::string &date) {
  std::vector<Segment> all_segments;
  std::vector<std::string> transport_types = {"plane", "train"};
  bool flag = false;

  for (const std::string &type : transport_types) {
    json json_data = MakeRequest(FromCode, ToCode, ApiKey, date, type);

    if (!json_data.is_null()) {
      flag = true;
      std::vector segments = ParseSegments(json_data, type);
      all_segments.insert(all_segments.end(), segments.begin(), segments.end());
    }
  }

  if (!flag)
    return;

  std::cout << "Количество рейсов: " << all_segments.size() << '\n';
  std::cout << "Список рейсов:\n";
  for (size_t i = 0; i < 72; ++i)
    std::cout << '-';
  std::cout << '\n';
  for (const Segment &s : all_segments) {
    std::cout << (s.transport_type == "plane" ? "Самолет\n" : "Поезд\n");
    std::cout << "Время и пункт отправления:\t" << s.depature << " - " << s.from
              << '\n';
    std::cout << "Время и пункт прибытия:   \t" << s.arrival << " - " << s.to
              << '\n';
    std::cout << "Подробнее о перевозчике: " << s.carrier;
    if (!s.url_carrier.empty())
      std::cout << "\t(" << s.url_carrier << ")\n";
    else
      std::cout << '\n';
    for (size_t i = 0; i < 72; ++i)
      std::cout << '-';
    std::cout << '\n';
  }
}
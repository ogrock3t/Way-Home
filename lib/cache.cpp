#include "cache.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>

std::string GetApiKey(const std::string &filename) {
  std::ifstream file(filename);

  if (file.is_open()) {
    json ApiKey;
    file >> ApiKey;

    if (ApiKey.contains("ApiKey")) {
      return ApiKey["ApiKey"].get<std::string>();
    }
  }

  return "";
}

std::string GenerateCacheKey(const std::string &FromCode,
                             const std::string &ToCode, const std::string &date,
                             const std::string &transport_type) {
  return FromCode + '_' + ToCode + '_' + date + '_' + transport_type;
}

bool IsCacheValid(const std::string &cache_key) {
  std::string cache_file = "./cache/" + cache_key + ".json";

  std::ifstream file(cache_file);
  if (!file.is_open()) {
    return false;
  }

  std::filesystem::file_time_type last_write_time =
      std::filesystem::last_write_time(cache_file);

  std::chrono::system_clock::time_point system_time =
      std::chrono::file_clock::to_sys(last_write_time);

  std::time_t current_time = std::time(nullptr);

  if (std::difftime(current_time, std::chrono::system_clock::to_time_t(
                                      system_time)) > 60 * 60) {
    return false;
  }

  return true;
}

json ReadCache(const std::string &cache_key) {
  std::string cache_file = "./cache/" + cache_key + ".json";

  std::ifstream file(cache_file);

  json data;

  if (file.is_open()) {
    file >> data;
  }

  return data;
}

void WriteCache(const std::string &cache_key, const json &data) {
  std::string cache_file = "./cache/" + cache_key + ".json";

  std::filesystem::create_directory("./cache/");

  std::ofstream file(cache_file);

  if (file.is_open()) {
    file << data.dump(4);
  }
}
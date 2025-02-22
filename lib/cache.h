#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

std::string GetApiKey(const std::string &filename);

std::string GenerateCacheKey(const std::string &FromCode,
                             const std::string &ToCode, const std::string &date,
                             const std::string &transport_type);

bool IsCacheValid(const std::string &cache_key);

json ReadCache(const std::string &cache_key);

void WriteCache(const std::string &cache_key, const json &data);
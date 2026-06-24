
#include "config.h"
#include <nlohmann/json.hpp>

using namespace databricks;

Config::Config(const std::string& fileName) : _file_name(fileName)
{
    std::ifstream file_config(_file_name);
    nlohmann::json j;
    file_config >> j;

    _conn_str = j.at(CONNECT_STR_NAME).get<std::string>();

    _api_key = j.at("dataBrickAPI").at("apiKey").get<std::string>();
    _base_url = j.at("dataBrickAPI").at("baseUrl").get<std::string>();
    _create_table_api = j.at("dataBrickAPI").at("createTableAPI").get<std::string>();
}

const std::string &Config::GetConnectStr() const
{
    return _conn_str;
}

const std::string &Config::GetAPIKey() const
{
    return _api_key;
}

const std::string &Config::GetBaseUrl() const
{
    return _base_url;
}

const std::string &Config::GetCreateTableAPI() const
{
    return _create_table_api;
}

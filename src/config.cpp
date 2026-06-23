
#include "config.h"
#include <nlohmann/json.hpp>

databricks::Config::Config(const char *file_name) : _file_name(file_name)
{
    std::ifstream file_config(_file_name.c_str());
    nlohmann::json j;
    file_config >> j;

    std::string connStr = j.at(CONNECT_STR_NAME).get<std::string>();

    _api_key = j.at("dataBrickAPI").at("apiKey").get<std::string>();
    _base_url = j.at("dataBrickAPI").at("baseUrl").get<std::string>();
    _create_table_api = j.at("dataBrickAPI").at("createTableAPI").get<std::string>();

    _conn_str = std::wstring(
        connStr.begin(),
        connStr.end());
}

const std::wstring &databricks::Config::GetConnectStr() const
{
    return _conn_str;
}

const std::string &databricks::Config::GetAPIKey() const
{
    return _api_key;
}

const std::string &databricks::Config::GetBaseUrl() const
{
    return _base_url;
}

const std::string &databricks::Config::GetCreateTableAPI() const
{
    return _create_table_api;
}

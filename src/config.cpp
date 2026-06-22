
#include "config.h"
#include <nlohmann/json.hpp>

databricks::Config::Config(const char *file_name) : _file_name(file_name)
{
    std::ifstream file_config(_file_name.c_str());
    nlohmann::json j;
    file_config >> j;
    
    std::string connStr =
        j[CONNECT_STR_NAME].get<std::string>();

    _conn_str = std::wstring(
        connStr.begin(),
        connStr.end()
    );
}

const std::wstring& databricks::Config::GetConnectStr() const {
    return _conn_str;
}

#pragma once

#include <string>
#include <fstream>

#define CONNECT_STR_NAME "connectStr"

namespace databricks
{
    class Config
    {
    private:
        std::string _file_name;
        std::wstring _conn_str;
        std::string _api_key;
        std::string _base_url;
        std::string _create_table_api;

    public:
        Config(const char *file_name);
        const std::wstring &GetConnectStr() const;
        const std::string &GetAPIKey() const;
        const std::string &GetBaseUrl() const;
        const std::string &GetCreateTableAPI() const;
    };
}

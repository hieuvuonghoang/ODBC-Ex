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
        std::string _conn_str;
        std::string _api_key;
        std::string _base_url;
        std::string _create_table_api;

    public:
        Config(const std::string& fileName);
        const std::string &GetConnectStr() const;
        const std::string &GetAPIKey() const;
        const std::string &GetBaseUrl() const;
        const std::string &GetCreateTableAPI() const;
    };
}

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

    public:
        Config(const char *file_name);
        const std::wstring &GetConnectStr() const;
    };
}

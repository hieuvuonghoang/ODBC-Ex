#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

#define CONNECT_STR_NAME "connectStr"

class Config
{
private:
    std::string _file_name;
    std::wstring _conn_str;
public:
    Config(const char *file_name);
    const std::wstring& GetConnectStr() const;
};
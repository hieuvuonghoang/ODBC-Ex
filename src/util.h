#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>

class Util
{
public:
    static std::string GetError(SQLSMALLINT handleType, SQLHANDLE handle);
    static std::string WStringToUtf8(const std::wstring &wstr);
    static std::string GetTypeName(SQLSMALLINT type, std::string columnName);
};
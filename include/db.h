
#pragma once

#include <windows.h>
#include <sqlext.h>
#include <iostream>
#include <string>

namespace databricks
{
    class Db
    {
    private:
        SQLHENV hEnv = SQL_NULL_HENV;
        SQLHDBC hDbc = SQL_NULL_HDBC;
        std::wstring connStr;

    public:
        Db(const wchar_t *connStr);
        SQLHDBC GetHDBC();
        bool Connect();
        void Disconnect();
        ~Db();
    };
}

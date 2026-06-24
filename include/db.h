
#pragma once

#include <windows.h>
#include <sql.h>
#include <iostream>
#include <string>

namespace databricks
{
    class Db
    {
    private:
        SQLHENV hEnv = SQL_NULL_HENV;
        SQLHDBC hDbc = SQL_NULL_HDBC;
        std::string connStr;

    public:
        Db(const std::string& connectionString);
        SQLHDBC GetHDBC() const;
        bool Connect();
        void Disconnect();
        ~Db();
    };
}

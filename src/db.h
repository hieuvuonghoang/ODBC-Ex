
#pragma once

#include <windows.h>
#include <sqlext.h>
#include <iostream>
#include <string>

class Db
{
private:
    SQLHENV hEnv = SQL_NULL_HENV;
    SQLHDBC hDbc = SQL_NULL_HDBC;
    SQLHSTMT hStmt = SQL_NULL_HSTMT;
    SQLRETURN ret;
    SQLWCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;
    std::wstring connStr;

public:
    Db(const wchar_t *connStr);
    bool Connect();
    ~Db();
};
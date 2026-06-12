#include "db.h"

Db::Db(const wchar_t *connStr)
    : connStr(connStr)
{

    SQLAllocHandle(
        SQL_HANDLE_ENV,
        SQL_NULL_HANDLE,
        &hEnv);

    SQLSetEnvAttr(
        hEnv,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER)SQL_OV_ODBC3,
        0);

    SQLAllocHandle(
        SQL_HANDLE_DBC,
        hEnv,
        &hDbc);
}

Db::~Db()
{
    if (hDbc != SQL_NULL_HDBC)
        SQLDisconnect(hDbc);

    if (hStmt != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    if (hDbc != SQL_NULL_HDBC)
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);

    if (hEnv != SQL_NULL_HENV)
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool Db::Connect()
{
    SQLRETURN ret;

    ret = SQLDriverConnectW(
        hDbc,
        NULL,
        (SQLWCHAR *)connStr.c_str(),
        SQL_NTS,
        NULL,
        0,
        NULL,
        SQL_DRIVER_NOPROMPT);

    if (!SQL_SUCCEEDED(ret))
        return false;

    return true;
}
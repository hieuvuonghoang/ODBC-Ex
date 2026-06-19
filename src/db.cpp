#include "db.h"
#include "util.h"

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
    Disconnect();

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
    {
        throw std::runtime_error(Util::GetError(SQL_HANDLE_DBC, hDbc));
    }

    return true;
}

void Db::Disconnect()
{
    if (hDbc != SQL_NULL_HDBC)
        SQLDisconnect(hDbc);
}

SQLHDBC Db::GetHDBC()
{
    return hDbc;
}
#include <windows.h>
#include <sqlext.h>
#include <iostream>

int main()
{
    SQLHENV hEnv = SQL_NULL_HENV;
    SQLWCHAR driverDesc[256];
    SQLWCHAR driverAttr[256];
    SQLSMALLINT descLen, attrLen;

    SQLUSMALLINT direction = SQL_FETCH_FIRST;

    while (SQLDriversW(
               hEnv,
               direction,
               driverDesc,
               256,
               &descLen,
               driverAttr,
               256,
               &attrLen) == SQL_SUCCESS)
    {
        std::wcout << driverDesc << std::endl;
        direction = SQL_FETCH_NEXT;
    }

    std::cout << "DONE";

    return 0;
}
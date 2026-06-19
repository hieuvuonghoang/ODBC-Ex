#include <iostream>
#include "db.h"

std::string GetTypeName(SQLSMALLINT);

int main()
{
    SQLHENV hEnv = SQL_NULL_HENV;    // Môi trường ODBC
    SQLHDBC hDbc = SQL_NULL_HDBC;    // Kết nối tới CSDL SQL Server
    SQLHSTMT hStmt = SQL_NULL_HSTMT; // Câu lệnh SQL
    SQLRETURN ret;

    // Tạo Environment Handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);

    ret = SQLSetEnvAttr(
        hEnv,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER)SQL_OV_ODBC3,
        0);

    // Tạo Connection Handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    SQLWCHAR connStr[] =
        L"Driver={ODBC Driver 18 for SQL Server};"
        L"Server=10.235.50.105,31433;"
        L"Database=FC40_CapacityControl;"
        L"UID=fc40;"
        L"PWD=Fc40@123456;"
        L"TrustServerCertificate=Yes;";

    SQLWCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

    // SQLDriverConnectW(SQLHDBC hdbc,SQLHWND hwnd,SQLWCHAR *szConnStrIn,SQLSMALLINT cbConnStrIn,SQLWCHAR *szConnStrOut,SQLSMALLINT cbConnStrOutMax,SQLSMALLINT *pcbConnStrOut,SQLUSMALLINT fDriverCompletion);
    ret = SQLDriverConnectW(
        hDbc,
        NULL,
        connStr,
        SQL_NTS,
        outConnStr,
        sizeof(outConnStr) / sizeof(SQLWCHAR),
        &outConnStrLen,
        SQL_DRIVER_NOPROMPT);

    if (!SQL_SUCCEEDED(ret))
    {
        std::wcout << L"Connection failed!" << std::endl;
    }
    else
    {
        std::wcout << L"Connected successfully!" << std::endl;

        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        ret = SQLExecDirectW(
            hStmt,
            (SQLWCHAR *)L"SELECT TOP (0) * FROM capacitycontrol.FC40_Documents ORDER BY Id DESC",
            SQL_NTS);

        SQLSMALLINT columnCount;
        SQLNumResultCols(hStmt, &columnCount);

        for (SQLSMALLINT i = 1; i <= columnCount; i++)
        {
            SQLWCHAR columnName[128];
            SQLSMALLINT nameLength;
            SQLSMALLINT dataType;
            SQLULEN columnSize;
            SQLSMALLINT decimalDigits;
            SQLSMALLINT nullable;

            SQLDescribeColW(
                hStmt,
                i,
                columnName,
                128,
                &nameLength,
                &dataType,
                &columnSize,
                &decimalDigits,
                &nullable);

            std::wcout
                << "Column: " << columnName
                << ", Type: " << dataType
                << ", TypeName: " << GetTypeName(dataType).c_str()
                << ", Size: " << columnSize
                << std::endl;
        }
    }

    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    std::cin.get();

    return 0;
}

std::string GetTypeName(SQLSMALLINT type)
{
    switch (type)
    {
        case SQL_CHAR: return "CHAR";
        case SQL_VARCHAR: return "VARCHAR";
        case SQL_WVARCHAR: return "NVARCHAR";
        case SQL_INTEGER: return "INT";
        case SQL_BIGINT: return "BIGINT";
        case SQL_DOUBLE: return "DOUBLE";
        case SQL_TIMESTAMP: return "DATETIME";
        default: return "UNKNOWN";
    }
}
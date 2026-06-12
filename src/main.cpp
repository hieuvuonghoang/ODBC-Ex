#include <windows.h>
#include <sqlext.h>
#include <iostream>

void ShowError(SQLSMALLINT handleType, SQLHANDLE handle);

int main()
{

    SQLHENV hEnv = SQL_NULL_HENV;    // Môi trường ODBC
    SQLHDBC hDbc = SQL_NULL_HDBC;    // Kết nối tới CSDL
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
        L"Server=10.235.4.146;"
        L"Database=SAP;"
        L"UID=userdev;"
        L"PWD=Wec!@#$1234;"
        L"TrustServerCertificate=Yes;";

    SQLWCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

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
        ShowError(SQL_HANDLE_DBC, hDbc);
    }
    else
    {
        std::wcout << L"Connected successfully!" << std::endl;

        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        ret = SQLExecDirectW(
            hStmt,
            (SQLWCHAR *)L"SELECT 1",
            SQL_NTS);

        SQLINTEGER id;
        SQLLEN indicator;
        while (SQLFetch(hStmt) != SQL_NO_DATA)
        {
            ret = SQLGetData(
                hStmt,
                1, // cột thứ 1
                SQL_C_LONG,
                &id,
                sizeof(id),
                &indicator);
            std::wcout << L"Ret Id: " << id << std::endl;
        }
    }

    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}

void ShowError(SQLSMALLINT handleType, SQLHANDLE handle)
{
    SQLWCHAR sqlState[6];
    SQLWCHAR message[1024];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;
    SQLRETURN ret;
    SQLSMALLINT i = 1;

    while ((ret = SQLGetDiagRecW(
                handleType,
                handle,
                i,
                sqlState,
                &nativeError,
                message,
                sizeof(message) / sizeof(SQLWCHAR),
                &textLength)) != SQL_NO_DATA)
    {
        std::wcout
            << L"SQLSTATE: " << sqlState
            << L", Native Error: " << nativeError
            << L", Message: " << message
            << std::endl;

        i++;
    }
}
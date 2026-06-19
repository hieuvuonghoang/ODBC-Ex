#include <iostream>
#include "db.h"

int main()
{
    wchar_t connStr[] =
        L"Driver={ODBC Driver 18 for SQL Server};"
        L"Server=10.235.50.105,31433;"
        L"Database=FC40_CapacityControl;"
        L"UID=fc40;"
        L"PWD=Fc40@123456;"
        L"TrustServerCertificate=Yes;";
    Db db(connStr);
    if (db.Connect())
    {
        std::wcout << L"Connected successfully!" << std::endl;
    }
    else
    {
        std::wcout << L"Connection failed!" << std::endl;
    }
    std::cin.get();
    // SQLHENV hEnv = SQL_NULL_HENV;    // Môi trường ODBC
    // SQLHDBC hDbc1 = SQL_NULL_HDBC;   // Kết nối tới CSDL SQL Server
    // SQLHDBC hDbc2 = SQL_NULL_HDBC;   // Kết nối tới CSDL MySQL
    // SQLHSTMT hStmt = SQL_NULL_HSTMT; // Câu lệnh SQL
    // SQLRETURN ret;

    // // Tạo Environment Handle
    // ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);

    // ret = SQLSetEnvAttr(
    //     hEnv,
    //     SQL_ATTR_ODBC_VERSION,
    //     (SQLPOINTER)SQL_OV_ODBC3,
    //     0);

    // // Tạo Connection Handle
    // ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc1);
    // ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc2);

    // SQLWCHAR connStr1[] =
    //     L"Driver={ODBC Driver 18 for SQL Server};"
    //     L"Server=10.235.4.146;"
    //     L"Database=SAP;"
    //     L"UID=userdev;"
    //     L"PWD=Wec!@#$1234;"
    //     L"TrustServerCertificate=Yes;";

    // SQLWCHAR outConnStr1[1024];
    // SQLSMALLINT outConnStrLen1;

    // ret = SQLDriverConnectW(
    //     hDbc1,
    //     NULL,
    //     connStr1,
    //     SQL_NTS,
    //     outConnStr1,
    //     sizeof(outConnStr1) / sizeof(SQLWCHAR),
    //     &outConnStrLen1,
    //     SQL_DRIVER_NOPROMPT);

    // if (!SQL_SUCCEEDED(ret))
    // {
    //     std::wcout << L"Connection failed!" << std::endl;
    //     ShowError(SQL_HANDLE_DBC, hDbc1);
    // }
    // else
    // {
    //     std::wcout << L"Connected successfully!" << std::endl;

    //     ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc1, &hStmt);

    //     ret = SQLExecDirectW(
    //         hStmt,
    //         (SQLWCHAR *)L"SELECT 1",
    //         SQL_NTS);

    //     SQLINTEGER id;
    //     SQLLEN indicator;
    //     while (SQLFetch(hStmt) != SQL_NO_DATA)
    //     {
    //         ret = SQLGetData(
    //             hStmt,
    //             1, // cột thứ 1
    //             SQL_C_LONG,
    //             &id,
    //             sizeof(id),
    //             &indicator);
    //         std::wcout << L"Ret Id: " << id << std::endl;
    //     }
    // }

    // // server=vineco-farm-dev-mysql-flex-1.mysql.database.azure.com;port=3306;userid='admindbflex';password='P@ssw0rd2024';database=staging_vef_manufacture;Convert Zero Datetime=true;Character Set=utf8mb4;
    // SQLWCHAR connStr2[] =
    //     L"Driver={MySQL ODBC 9.7 Unicode Driver};"
    //     L"Server=vineco-farm-dev-mysql-flex-1.mysql.database.azure.com;"
    //     L"Port=3306;"
    //     L"Database=staging_vef_manufacture;"
    //     L"UID=admindbflex;"
    //     L"PWD=P@ssw0rd2024;";

    // SQLWCHAR outConnStr2[1024];
    // SQLSMALLINT outConnStrLen2;

    // ret = SQLDriverConnectW(
    //     hDbc2,
    //     NULL,
    //     connStr2,
    //     SQL_NTS,
    //     outConnStr2,
    //     sizeof(outConnStr2) / sizeof(SQLWCHAR),
    //     &outConnStrLen2,
    //     SQL_DRIVER_NOPROMPT);

    // if (!SQL_SUCCEEDED(ret))
    // {
    //     std::wcout << L"Connection failed!" << std::endl;
    //     ShowError(SQL_HANDLE_DBC, hDbc2);
    // }
    // else
    // {
    //     std::wcout << L"Connected successfully!" << std::endl;

    //     ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc2, &hStmt);

    //     ret = SQLExecDirectW(
    //         hStmt,
    //         (SQLWCHAR *)L"SELECT COUNT(*) FROM BestInClasses",
    //         SQL_NTS);

    //     SQLINTEGER id;
    //     SQLLEN indicator;
    //     while (SQLFetch(hStmt) != SQL_NO_DATA)
    //     {
    //         ret = SQLGetData(
    //             hStmt,
    //             1, // cột thứ 1
    //             SQL_C_LONG,
    //             &id,
    //             sizeof(id),
    //             &indicator);
    //         std::wcout << L"Ret Id: " << id << std::endl;
    //     }
    // }

    // SQLDisconnect(hDbc1);
    // SQLDisconnect(hDbc2);
    // SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    // SQLFreeHandle(SQL_HANDLE_DBC, hDbc1);
    // SQLFreeHandle(SQL_HANDLE_DBC, hDbc2);
    // SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}


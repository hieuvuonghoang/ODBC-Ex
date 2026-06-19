
#include <iostream>
#include <string>
#include <windows.h>
#include <sqlext.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <stdexcept>
#include <conio.h>
#include <fstream>

using json = nlohmann::json;

std::string GetTypeName(SQLSMALLINT, std::string);
std::vector<json> GetColumns(SQLWCHAR *);
std::string GetError(SQLSMALLINT, SQLHANDLE);
std::string WStringToUtf8(const std::wstring &);
std::string ReadConnectionString();
std::wstring Utf8ToWString(const std::string &);

SQLHENV hEnv = SQL_NULL_HENV;    // Môi trường ODBC
SQLHDBC hDbc = SQL_NULL_HDBC;    // Kết nối tới CSDL SQL Server
SQLHSTMT hStmt = SQL_NULL_HSTMT; // Câu lệnh SQL
SQLRETURN ret;

int main()
{
    try
    {
        std::string connStr = ReadConnectionString();
        std::wstring wConnStr = Utf8ToWString(connStr);
        ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv); // Tạo Environment Handle
        ret = SQLSetEnvAttr(
            hEnv,
            SQL_ATTR_ODBC_VERSION,
            (SQLPOINTER)SQL_OV_ODBC3,
            0);
        ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc); // Tạo Connection Handle
        SQLWCHAR outConnStr[1024];
        SQLSMALLINT outConnStrLen;
        ret = SQLDriverConnectW(
            hDbc,
            NULL,
            (SQLWCHAR *)wConnStr.c_str(),
            SQL_NTS,
            outConnStr,
            sizeof(outConnStr) / sizeof(SQLWCHAR),
            &outConnStrLen,
            SQL_DRIVER_NOPROMPT);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(GetError(SQL_HANDLE_DBC, hDbc));
        }
        std::vector<std::wstring> tables;
        tables.push_back(L"capacitycontrol.FC40_DocumentDetails");
        tables.push_back(L"capacitycontrol.FC40_Documents");
        for (size_t i = 0; i < tables.size(); i++)
        {
            std::wstring table_name = tables.at(i);
            std::wstring sql =
                L"SELECT TOP (0) * "
                L"FROM " +
                table_name +
                L" ORDER BY Id ASC";
            std::vector<json> columns = GetColumns((SQLWCHAR *)sql.c_str());
            std::wcout << sql << std::endl;
        }
        // capacitycontrol.FC40_DocumentDetails
        // std::wstring tableName = L"capacitycontrol.FC40_DocumentDetails";
        // std::wstring sql =
        //     L"SELECT TOP (0) * "
        //     L"FROM " +
        //     tableName +
        //     L" ORDER BY Id DESC";
        // std::vector<json> columns = GetColumns((SQLWCHAR *)sql.c_str());
        // if (columns.size() != 0)
        // {
        //     json j = json({{"system_name", "wineco"},
        //                    {"business_unit", "wineco"},
        //                    {"columns", columns}});
        //     std::cout << j.dump(2) << std::endl;
        // }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Err: " << ex.what() << std::endl;
    }
    // Free
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    std::wcout << L"Press any key to exit...";
    _getch();
    return 0;
}

std::string ReadConnectionString()
{
    std::ifstream file("config.json");

    json j;
    file >> j;

    return j["connectStr"];
}

std::string GetTypeName(SQLSMALLINT type, std::string columnName)
{
    switch (type)
    {
    case SQL_TYPE_DATE:
    case SQL_TYPE_TIME:
    case SQL_TYPE_TIMESTAMP:
    case SQL_TIMESTAMP:
    case SQL_DATETIME:
    case SQL_TIME:
        return "TIMESTAMP";
    case SQL_CHAR:
    case SQL_VARCHAR:
    case SQL_WVARCHAR:
        return "STRING";
    case SQL_INTEGER:
        return "INT";
    case SQL_BIGINT:
        return "BIGINT";
    case SQL_DOUBLE:
        return "DOUBLE";
    case SQL_BIT:
        return "SMALLINT";
    case SQL_BINARY:
    case SQL_VARBINARY:
    case SQL_LONGVARBINARY:
        return "BINARY";
    case SQL_DECIMAL:
        return "DECIMAL";
    default:
        throw std::runtime_error("Column " + columnName + ", type = " + std::to_string(type) + " not implement!");
    }
}

std::vector<json> GetColumns(SQLWCHAR *sql)
{
    std::vector<json> ans;
    try
    {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(GetError(SQL_HANDLE_STMT, hDbc));
        }
        ret = SQLExecDirectW(
            hStmt,
            sql,
            SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(GetError(SQL_HANDLE_STMT, hStmt));
        }
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

            std::wstring wsColumnName(
                reinterpret_cast<wchar_t *>(columnName),
                nameLength);

            std::string columnNameUtf8 = WStringToUtf8(wsColumnName);

            json jcolumn = json({
                {"field_name", columnNameUtf8},
                {"data_type", GetTypeName(dataType, columnNameUtf8)},
                {"description", ""},
                {"pii_tag", "No"},
                {"default_value", ""},
            });
            ans.push_back(jcolumn);
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Err: " << ex.what() << std::endl;
        ans.clear();
    }
    return ans;
}

std::string WStringToUtf8(const std::wstring &wstr)
{
    if (wstr.empty())
        return "";

    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.c_str(),
        -1,
        NULL,
        0,
        NULL,
        NULL);

    std::string result(size - 1, '\0');

    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.c_str(),
        -1,
        &result[0],
        size,
        NULL,
        NULL);

    return result;
}

std::wstring Utf8ToWString(const std::string &str)
{
    if (str.empty())
        return L"";

    int len = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        NULL,
        0);

    std::wstring result(len - 1, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        &result[0],
        len);

    return result;
}

std::string GetError(SQLSMALLINT handleType, SQLHANDLE handle)
{
    std::ostringstream oss;
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
        std::wstring wsState(sqlState);
        std::wstring wsMessage(message);

        oss << "SQLSTATE: "
            << std::string(wsState.begin(), wsState.end())
            << ", Native Error: "
            << nativeError
            << ", Message: "
            << std::string(wsMessage.begin(), wsMessage.end())
            << '\n';
        i++;
    }
    return oss.str();
}

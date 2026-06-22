
#include "util.h"

std::string Util::GetTypeName(SQLSMALLINT type, std::string columnName)
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

std::wstring Util::Utf8ToWString(const std::string &str)
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

std::string Util::WStringToUtf8(const std::wstring &wstr)
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

std::string Util::GetError(SQLSMALLINT handleType, SQLHANDLE handle)
{
    std::string result;
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

        result += "[" + std::to_string(i) + "] ";
        result += "SQLSTATE=";
        result += std::string(wsState.begin(), wsState.end());
        result += ", NativeError=";
        result += std::to_string(nativeError);
        result += ", Message=";
        result += std::string(wsMessage.begin(), wsMessage.end());
        result += "\n";

        i++;
    }
    return result;
}
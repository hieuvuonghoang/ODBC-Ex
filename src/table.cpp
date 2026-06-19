#include "table.h"

Table::Table(std::wstring name)
{
    this->name = name;
}

void Table::SetColumns(SQLHDBC hDbc)
{
    this->columns.clear();
    this->columns.shrink_to_fit();
    SQLHSTMT hStmt = SQL_NULL_HSTMT;
    try
    {
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(Util::GetError(SQL_HANDLE_DBC, hDbc));
        }
        std::wstring sql =
            L"SELECT TOP (0) * "
            L"FROM " +
            this->name +
            L" ORDER BY Id ASC";
        ret = SQLExecDirectW(
            hStmt,
            (SQLWCHAR *)sql.c_str(),
            SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(Util::GetError(SQL_HANDLE_STMT, hStmt));
        }
        SQLSMALLINT columnCount;
        ret = SQLNumResultCols(hStmt, &columnCount);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(Util::GetError(SQL_HANDLE_STMT, hStmt));
        }
        for (SQLSMALLINT i = 1; i <= columnCount; i++)
        {
            SQLWCHAR columnName[128];
            SQLSMALLINT nameLength;
            SQLSMALLINT dataType;
            SQLULEN columnSize;
            SQLSMALLINT decimalDigits;
            SQLSMALLINT nullable;
            ret = SQLDescribeColW(
                hStmt,
                i,
                columnName,
                128,
                &nameLength,
                &dataType,
                &columnSize,
                &decimalDigits,
                &nullable);

            if (!SQL_SUCCEEDED(ret))
            {
                throw std::runtime_error(Util::GetError(SQL_HANDLE_STMT, hStmt));
            }

            std::wstring wsColumnName(
                reinterpret_cast<wchar_t *>(columnName),
                nameLength);

            std::string columnNameUtf8 = Util::WStringToUtf8(wsColumnName);
            std::string dataTypeName = Util::GetTypeName(dataType, columnNameUtf8);

            this->columns.push_back(Column(columnNameUtf8, dataTypeName));
        }
    }
    catch (std::exception &e)
    {
        if (hStmt != SQL_NULL_HSTMT)
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        throw;
    }
    if (hStmt != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

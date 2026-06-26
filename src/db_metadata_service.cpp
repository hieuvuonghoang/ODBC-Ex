
#include <windows.h>
#include <sql.h>

#include "db_metadata_service.h"
#include "util.h"

using namespace databricks;

DbMetadataService::DbMetadataService(const Db& db) : _db(db)
{
};

std::vector<Column> DbMetadataService::GetColumns(const std::string &tableName) const
{
    std::vector<Column> result;
    SQLHSTMT hStmt = SQL_NULL_HSTMT;
    try
    {
        auto hdbc = _db.GetHDBC();
        SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);
        if (!SQL_SUCCEEDED(ret))
        {
            throw std::runtime_error(Util::GetError(SQL_HANDLE_DBC, hdbc));
        }
        std::wstring sql =
            L"SELECT TOP (0) * "
            L"FROM " +
            Util::Utf8ToWString(tableName);
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

            result.push_back(Column(columnNameUtf8, dataTypeName));
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
    return result;
}
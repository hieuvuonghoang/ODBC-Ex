#include "table.h"

Table::Table(std::string name)
{
    this->name = name;
}

Table::Table(std::string name,
             std::string system_name,
             std::string business_unit,
             std::string system_owner,
             std::string table_name,
             std::string data_owner,
             std::string partition_information,
             std::string synchronization_type)
    : name(std::move(name)),
      system_name(std::move(system_name)),
      business_unit(std::move(business_unit)),
      system_owner(std::move(system_owner)),
      table_name(std::move(table_name)),
      data_owner(std::move(data_owner)),
      partition_information(std::move(partition_information)),
      synchronization_type(std::move(synchronization_type))
{
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
            Util::Utf8ToWString(this->name) +
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

std::vector<Table> Table::GetTables(const char *file_name)
{
    std::vector<Table> result;
    rapidcsv::Document doc(file_name);
    size_t row_count = doc.GetRowCount();
    std::vector<std::string> table_name_in_db = doc.GetColumn<std::string>("table_name_in_db");
    std::vector<std::string> table_name_in_databrick = doc.GetColumn<std::string>("table_name_in_databrick");
    std::vector<std::string> system_name = doc.GetColumn<std::string>("system_name");
    std::vector<std::string> business_unit = doc.GetColumn<std::string>("business_unit");
    std::vector<std::string> system_owner = doc.GetColumn<std::string>("system_owner");
    std::vector<std::string> data_owner = doc.GetColumn<std::string>("data_owner");
    std::vector<std::string> partition_information = doc.GetColumn<std::string>("partition_information");
    std::vector<std::string> synchronization_type = doc.GetColumn<std::string>("synchronization_type");
    for (size_t row = 0; row < row_count; row++)
    {
        Table table = Table(
            table_name_in_db.at(row),
            system_name.at(row),
            business_unit.at(row),
            system_owner.at(row),
            table_name_in_databrick.at(row),
            data_owner.at(row),
            partition_information.at(row),
            synchronization_type.at(row)
        );
        result.push_back(table);
    }
    return result;
}

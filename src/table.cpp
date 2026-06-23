#include "table.h"
#include <rapidcsv.h>
#include <nlohmann/json.hpp>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

databricks::Table::Table(std::string name)
{
    this->name = name;
}

databricks::Table::Table(std::string name,
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

void databricks::Table::SetColumns(SQLHDBC hDbc)
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

std::string databricks::Table::GetBody()
{
    nlohmann::json jColumns = {
        {"columns", this->columns}};

    nlohmann::json j = {
        {"system_name", this->system_name},
        {"business_unit", this->business_unit},
        {"system_owner", this->system_owner},
        {"table_name", this->table_name},
        {"data_owner", this->data_owner},
        {"partition_information", this->partition_information},
        {"synchronization_type", this->synchronization_type},
        {"data_schema", jColumns}};

    return j.dump();
}

void databricks::Table::CreateTable(const Config *config)
{
    httplib::Client cli(config->GetBaseUrl());
    httplib::Request req;
    req.method = "POST";
    req.path = config->GetCreateTableAPI();
    req.body = this->GetBody();
    req.set_header("Content-Type", "application/json");
    req.set_header("ocp-apim-subscription-key", config->GetAPIKey());
    auto res = cli.send(req);
    this->success = false;
    if (res)
    {
        if (res->status == 200)
        {
            this->success = true;
            nlohmann::json j = nlohmann::json::parse(res->body);
            this->table_id = j.at("table_id").get<std::string>();
            this->mes = j.at("message").get<std::string>();
            // std::cout << "Table ID: " << this->table_id << "\n";
        }
        else if (res->status == 500)
        {
            nlohmann::json j = nlohmann::json::parse(res->body);
            this->mes = j.at("detail").get<std::string>();
            // std::cout << "Error: " << this->mes << "\n";
        }
        else
        {
            std::cout << "Status: " << res->status << "\n";
            nlohmann::json j = nlohmann::json::parse(res->body);
            std::cout << "Error: " << j.dump(2) << "\n";
        }
    }
    else
    {
        this->mes = "Request failed!";
    }
}

std::vector<databricks::Table> databricks::Table::GetTables(const char *file_name)
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
            synchronization_type.at(row));
        result.push_back(table);
    }
    return result;
}

void databricks::Table::SaveCsv(const std::vector<Table>& tables, const char* file_name)
{
    std::ofstream file(file_name);

    if (!file.is_open()) {
        std::cout << "Cannot open file!\n";
        return;
    }

    // Header
    file << "table_name_in_db,table_name_in_databrick,table_id,success,message\n";

    for (const auto& table : tables) {
        file << table.name << ","
             << table.table_name << ","
             << table.table_id << ","
             << table.success << ","
             << table.mes
             << "\n";
    }

    file.close();
}

#include "table.h"
#include <rapidcsv.h>
#include <nlohmann/json.hpp>

using namespace databricks;

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

void Table::SetColumns(std::vector<Column> columns)
{
    this->columns = std::move(columns);
}

std::string Table::GetBody() const
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

std::string Table::ToCsvRow() const {
    std::ostringstream oss;

    oss << name << ","
    << table_name << ","
    << table_id << ","
    << success << ","
    << mes << "\n";

    return oss.str();
}

const std::string& Table::GetTableName() const {
    return name;
}

void Table::SetTableId(std::string tableId) {
    table_id = tableId;
}

void Table::SetSuccess(bool isSuccess) {
    success = isSuccess;
}

void Table::SetMes(std::string message) {
    mes = message;
}

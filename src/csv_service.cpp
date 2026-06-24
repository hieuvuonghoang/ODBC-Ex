
#include "csv_service.h"
#include <rapidcsv.h>

using namespace databricks;

CSVService::CSVService() {

};

std::vector<Table> CSVService::ReadDatas(const std::string &fileName)
{
    std::vector<Table> result;
    rapidcsv::Document doc(fileName);
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

void CSVService::WriteDatas(const std::vector<Table> &tables, const std::string &fileName)
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cout << "Err: Cannot open file!\n";
        return;
    }

    // Header
    file << "table_name_in_db,table_name_in_databrick,table_id,success,message\n";

    for (const auto &table : tables)
    {
        file << table.ToCsvRow();
    }

    file.close();
}

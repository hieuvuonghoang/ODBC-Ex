#include <iostream>
#include <conio.h>
#include "db.h"
#include "config.h"
#include "db_metadata_service.h"
#include "csv_service.h"
#include "dbr_api_service.h"

using namespace databricks;

int main(int argc, char *argvs[])
{
    try
    {
        const std::string configFile = "config.json";
        const std::string inputFile = "input.csv";
        const std::string outPutFile = "output.csv";
        Config config(configFile);
        DBRApiService dbrApiService(config);
        
        Db db(config.GetConnectStr());
        db.Connect();
        std::cout << "Connect DB success!" << std::endl;

        DbMetadataService dbMetadataService(db);
        CSVService csvService;

        auto tables = csvService.ReadDatas(inputFile);

        for (auto &table : tables)
        {
            auto tableName = table.GetTableName();
            std::cout << "Table name: " << tableName << std::endl;
            std::cout << "\tGet columns." << std::endl;
            auto columns = dbMetadataService.GetColumns(tableName);
            std::cout << "\tSet columns." << std::endl;
            table.SetColumns(columns);
            std::cout << "\tCreate table." << std::endl;
            dbrApiService.CreateTable(table);
        }

        csvService.WriteDatas(tables, outPutFile);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

#include <iostream>
#include <conio.h>
#include "db.h"
#include "config.h"
#include "table.h"
#include "util.h"
#include "column.h"

int main()
{
    try
    {
        Config config("config.json");

        Db db(config.GetConnectStr().c_str());

        db.Connect();

        std::vector<Table> tables;

        tables.emplace_back(
            L"capacitycontrol.FC40_Documents");

        tables.emplace_back(
            L"capacitycontrol.FC40_DocumentDetails");

        for (auto &table : tables)
        {
            table.SetColumns(db.GetHDBC());
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    std::cout << "Press any key to exit...";
    _getch();
}

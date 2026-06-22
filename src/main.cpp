#include <iostream>
#include <conio.h>
#include "db.h"
#include "config.h"
#include "table.h"

int main(int argc, char *argvs[])
{
    try
    {
        Config config("config.json");

        Db db(config.GetConnectStr().c_str());

        db.Connect();

        std::vector<Table> tables = Table::GetTables("input.csv");

        for (auto &table : tables)
        {
            table.SetColumns(db.GetHDBC());
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        _getch();
        return 1;
    }
    std::cout << "Press any key to exit...";
    _getch();
}

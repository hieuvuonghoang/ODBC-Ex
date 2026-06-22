
#include <rapidcsv.h>
#include <string>
#include <iostream>
#include <vector>
#include "util.h"

int main()
{
    rapidcsv::Document doc("input.csv");
    std::cout << doc.GetRowCount() << std::endl;
    // std::vector<std::string> col = doc.GetColumn<std::string>("table_name_in_db");
    // for (auto &item : col)
    // {
    //     std::wstring wstr = Util::Utf8ToWString(item);
    //     std::wcout << wstr << std::endl;
    // }
    // std::cout << "Read " << col.size() << " values." << std::endl;
    return 0;
}
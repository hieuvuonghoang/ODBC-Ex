#pragma once

#include <vector>
#include "table.h"

namespace databricks
{
    class CSVService
    {
    private:
        /* data */
    public:
        CSVService();
        std::vector<Table> ReadDatas(const std::string& fileName);
        void WriteDatas(const std::vector<Table>& tables, const std::string& fileName);
    };

}
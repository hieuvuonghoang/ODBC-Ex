#pragma once

#include <vector>
#include "column.h"
#include "db.h"

namespace databricks
{
    class DbMetadataService
    {
    private:
        const Db& _db;
    public:
        DbMetadataService(const Db& db);
        std::vector<Column> GetColumns(const std::string &tableName) const;
    };
}


#pragma once

#include <string>
#include "config.h"
#include "table.h"

namespace databricks
{
    class DBRApiService
    {
        private:
            std::string apiKey;
            std::string baseUrl;
            std::string createTableEndpoint;
        public:
            DBRApiService(const Config& config);
            void CreateTable(Table& table) const;
    };
}
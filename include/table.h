#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <sql.h>
#include "column.h"
#include "util.h"
#include "config.h"

namespace databricks
{

    class Table
    {
    private:
        std::string name;
        std::string system_name;
        std::string business_unit;
        std::string system_owner;
        std::string table_name;
        std::string data_owner;
        std::string partition_information;
        std::string synchronization_type;
        std::string table_id;
        bool success;
        std::string mes;
        std::vector<Column> columns;

    public:
        /// @brief
        /// @param name
        Table(std::string name);
        /// @brief
        /// @param name
        /// @param system_name
        /// @param business_unit
        /// @param system_owner
        /// @param table_name
        /// @param data_owner
        /// @param partition_information
        /// @param synchronization_type
        Table(
            std::string name,
            std::string system_name,
            std::string business_unit,
            std::string system_owner,
            std::string table_name,
            std::string data_owner,
            std::string partition_information,
            std::string synchronization_type);
        /// @brief 
        /// @return 
        const std::string& GetTableName() const;
        /// @brief 
        /// @return 
        std::string GetBody() const;
        /// @brief 
        /// @return 
        std::string ToCsvRow() const;
        /// @brief
        /// @param columns
        void SetColumns(std::vector<Column> columns);
        /// @brief 
        /// @param tableId 
        void SetTableId(std::string tableId);
        /// @brief 
        /// @param mes 
        void SetMes(std::string mes);
        /// @brief 
        /// @param success 
        void SetSuccess(bool success);
    };
}

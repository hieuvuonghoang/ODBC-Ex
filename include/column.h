#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

namespace databricks
{
    class Column
    {
    public:
        Column(std::string field_name, std::string data_type);
        std::string field_name;
        std::string data_type;
        std::string description;
        std::string default_value;
        std::string pii_tag;

        friend void to_json(nlohmann::json &j, const Column &c)
        {
            j = nlohmann::json{
                {"field_name", c.field_name},
                {"data_type", c.data_type},
                {"description", c.description},
                {"default_value", c.default_value},
                {"pii_tag", c.pii_tag},
            };
        }
    };
}

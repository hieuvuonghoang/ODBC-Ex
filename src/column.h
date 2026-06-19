#pragma once

#include <string>
#include <iostream>

class Column {
public:
    Column(std::string field_name, std::string data_type);
    std::string field_name;
    std::string data_type;
    std::string description;
    std::string default_value;
    std::string pii_tag;
};

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

int main()
{
    json j_column = json({
        {"field_name", "user_user_id"},
        {"data_type", "string"},
        {"description", "user_id"},
        {"pii_tag", "No"},
        {"default_value", ""},
    });
    
    std::vector<json> column_vec;
    column_vec.push_back(j_column);
    column_vec.push_back(j_column);

    json j = json({
        {"system_name", "wineco"},
        {"business_unit", "wineco"},
        {"columns", column_vec}
    });
    
    std::cout << j.dump(4) << std::endl;
    return 0;
}
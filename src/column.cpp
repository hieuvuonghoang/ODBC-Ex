
#include "column.h"

databricks::Column::Column(std::string field_name, std::string data_type)
: field_name(std::move(field_name)), data_type(std::move(data_type))
{
}
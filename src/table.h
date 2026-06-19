#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <sql.h>
#include "column.h"
#include "util.h"

class Table
{
private:
    std::wstring name;
    std::vector<Column> columns;
public:
    Table(std::wstring name);
    void SetColumns(SQLHDBC hDBC);
};
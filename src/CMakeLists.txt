
cmake_minimum_required(VERSION 4.3)

project(CreateTable)

#

add_library(rapidcsv INTERFACE)
target_include_directories(rapidcsv INTERFACE include)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE include)

#
add_library(Util STATIC
    util.cpp
    util.h
)

target_link_libraries(Util
    PRIVATE
        odbc32
        odbccp32
)

target_include_directories(Util PUBLIC include)

#
add_library(Db STATIC
    db.cpp
    db.h
)

target_link_libraries(Db
    PRIVATE
        odbc32
        odbccp32
)

target_include_directories(Db PUBLIC include)

#
add_library(Config STATIC
    config.cpp
    config.h
)

target_link_libraries(Config
    PRIVATE
        nlohmann_json
)

target_include_directories(Config PUBLIC include)

#
add_library(Table STATIC
    table.cpp
    table.h
)

target_link_libraries(Table
    PRIVATE
        rapidcsv
        Util
        Column
)

target_include_directories(Table PUBLIC include)

#
add_library(Column STATIC
    column.cpp
    column.h
)

target_include_directories(Column PUBLIC include)

#
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Db
        Config
        Table
)

cmake_minimum_required(VERSION 4.3)

project(CreateTable)

add_executable(${PROJECT_NAME})

target_sources(CreateTable
    PRIVATE
        main.cpp
)

#

add_library(rapidcsv INTERFACE)

target_link_libraries(${PROJECT_NAME} PRIVATE rapidcsv)

target_sources(rapidcsv
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
        include
    FILES
        include/rapidcsv.h
)

add_library(nlohmann_json INTERFACE)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)

target_sources(nlohmann_json
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
        include
    FILES
        include/json.hpp
)

#
target_link_libraries(${PROJECT_NAME} PRIVATE Db)

add_library(Db)

target_sources(Db
  PRIVATE
    db.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      db.h
)

target_link_libraries(Db
    PRIVATE
        odbc32
        odbccp32
)

#

target_link_libraries(${PROJECT_NAME} PRIVATE Config)

add_library(Config)

target_sources(Config
  PRIVATE
    config.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      config.h
)

target_link_libraries(Config PRIVATE nlohmann_json)

#
target_link_libraries(${PROJECT_NAME} PRIVATE Table)

add_library(Table)

target_sources(Table
  PRIVATE
    table.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      table.h
)

target_link_libraries(Table PRIVATE rapidcsv)

#
target_link_libraries(Table PRIVATE Util)

add_library(Util)

target_sources(Util
  PRIVATE
    util.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      util.h
)

target_link_libraries(Util
    PRIVATE
        odbc32
        odbccp32
)

#
target_link_libraries(Table PRIVATE Column)

add_library(Column)

target_sources(Column
  PRIVATE
    column.cpp

  PUBLIC
    FILE_SET HEADERS
    FILES
      column.h
)

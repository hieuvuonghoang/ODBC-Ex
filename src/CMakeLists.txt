
#

add_library(databricklib STATIC
    util.cpp
    db.cpp
    table.cpp
    column.cpp
    config.cpp
)

target_link_libraries(databricklib PRIVATE libs odbc32 odbccp32)

target_include_directories(databricklib PUBLIC ${CMAKE_SOURCE_DIR}/include)

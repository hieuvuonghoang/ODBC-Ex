
#

add_library(databrick STATIC
    util.cpp
    db.cpp
    table.cpp
    column.cpp
    config.cpp
)

target_link_libraries(databrick PRIVATE libs odbc32 odbccp32)

target_include_directories(databrick PUBLIC ${CMAKE_SOURCE_DIR}/include)

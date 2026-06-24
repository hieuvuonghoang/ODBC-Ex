
#

add_library(databrick STATIC
    util.cpp
    db.cpp
    table.cpp
    column.cpp
    config.cpp
    db_metadata_service.cpp
    csv_service.cpp
    dbr_api_service.cpp
)


target_link_libraries(databrick PUBLIC 
    libs)

target_link_libraries(databrick PRIVATE  
    odbc32 
    odbccp32 
    ws2_32
    ssl
    crypto
    crypt32)

target_compile_definitions(databrick PRIVATE _WIN32_WINNT=0x0A00)

target_include_directories(databrick PUBLIC ${CMAKE_SOURCE_DIR}/include)

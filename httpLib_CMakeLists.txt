
cmake_minimum_required(VERSION 4.3)

project(httplib)



#
add_subdirectory(libs)

#

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/app/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    libs
    ws2_32
    ssl
    crypto
    crypt32
)

target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0A00)

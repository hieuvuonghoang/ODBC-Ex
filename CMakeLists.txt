
cmake_minimum_required(VERSION 4.3)

project(CreateTable)

#
add_subdirectory(libs)

#
add_subdirectory(src)

#

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/app/main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        databricklib
)

#

set(RESOURCE_FILES
    config.json
    input.csv
)

foreach(file ${RESOURCE_FILES})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/app/${file}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${file}
    )
endforeach()


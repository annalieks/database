cmake_minimum_required(VERSION 3.17)
project(database C)

set(CMAKE_C_STANDARD 11)

list(APPEND SOURCES
        src/main.c
        src/database.c
        src/interface.c)

add_executable(database ${SOURCES})
target_include_directories(database PUBLIC include/)

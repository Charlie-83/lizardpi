cmake_minimum_required(VERSION 3.14)
project(LizardPi)

add_subdirectory(libepaper)

add_executable(main main.c)
target_link_libraries(main libepaper)

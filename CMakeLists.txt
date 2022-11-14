cmake_minimum_required(VERSION 3.18)
project(PoS_2022 CXX)

set(CMAKE_CXX_STANDARD 11)

#add_executable(PoS_2022
#        "Cvičenie t7/main.c"
#         "Cvičenie t7/date.c"
#         "Cvičenie t7/date.h"
#         "Cvičenie t7/array_list.c"
#         "Cvičenie t7/array_list.h")

#add_executable(Cvicenie_T08_01
#        "Cvičenie t8/Uloha 1.c")

add_executable(Cvicenie_T08_02
        "Cvičenie t8/Uloha 2.cpp")

add_executable(Cvicenie_T08_03
        "Cvičenie t8/Uloha 3.cpp")

add_executable(Cvicenie_T09_01
        "Cvičenie t9/cvicenie T9 - 01.cpp")

target_link_libraries(Cvicenie_T09_01 pthread)

add_executable(Cvicenie_T09_02
        "Cvičenie t9/cvicenie T9 - 02.cpp")

target_link_libraries(Cvicenie_T09_02 pthread)
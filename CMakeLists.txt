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

#[[add_executable(Cvicenie_T08_02
        "Cvičenie t8/Uloha 2.cpp")

add_executable(Cvicenie_T08_03
        "Cvičenie t8/Uloha 3.cpp")

add_executable(Cvicenie_T09_01
        "Cvičenie t9/cvicenie T9 - 01.cpp")

target_link_libraries(Cvicenie_T09_01 pthread)

add_executable(Cvicenie_T09_02
        "Cvičenie t9/cvicenie T9 - 02.cpp")

target_link_libraries(Cvicenie_T09_02 pthread)]]

add_executable(Cvicenie_T10_01
        "Cvičenie t10/cvicenie T10 - 1.cpp")
target_link_libraries(Cvicenie_T10_01 pthread)

add_executable(Precvicovanie2-25
        "Precvicovanie2/Pr25_SusenieHub.cpp")
target_link_libraries(Precvicovanie2-25 pthread)

add_executable(Precvicovanie2-01
        "Precvicovanie2/Pr01_Prvocisla.cpp")
target_link_libraries(Precvicovanie2-01 pthread)

add_executable(Precvicovanie2-02
        "Precvicovanie2/Pr02_KombinacneCisla.cpp")
target_link_libraries(Precvicovanie2-02 pthread)

add_executable(Precvicovanie2-vlastne
        "Precvicovanie2/Pr_vlastne.cpp")
target_link_libraries(Precvicovanie2-vlastne pthread)

add_executable(Pisomka2-29-11
        "Pisomka2/Tobias_Mitala_5ZYI33.cpp")
target_link_libraries(Pisomka2-29-11 pthread)
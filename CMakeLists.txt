# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.9)
project(sonar_oculus_m750d
        VERSION 0.1
        DESCRIPTION "")
find_package(Rock)
rock_find_qt5(REQUIRED COMPONENTS Core Network Widgets)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

rock_init()
rock_standard_layout()
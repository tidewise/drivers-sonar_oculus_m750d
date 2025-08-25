rock_library(sonar_oculus_m750d
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(sonar_oculus_m750d_bin Main.cpp
    DEPS sonar_oculus_m750d)


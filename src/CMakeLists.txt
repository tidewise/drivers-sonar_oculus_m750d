rock_library(sonar_oculus_m750d
    SOURCES Dummy.cpp

target_link_libraries(sonar_oculus_m750d PRIVATE Qt5::Core Qt5::Network Qt5::Widgets)


rock_executable(sonar_oculus_m750d_bin Main.cpp
    DEPS sonar_oculus_m750d)


rock_library(sonar_oculus_m750d
    SOURCES Driver.cpp
            Protocol.cpp
    HEADERS Driver.hpp
            Protocol.hpp
            Oculus.h
            M750DConfiguration.hpp
            SonarData.hpp
    DEPS_PKGCONFIG base-types iodrivers_base)

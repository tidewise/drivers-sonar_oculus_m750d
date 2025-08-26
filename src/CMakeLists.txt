rock_library(sonar_oculus_m750d
    SOURCES RmUtil/RmUtil.cpp
            Oculus/OsClientCtrl.cpp
            Oculus/OsStatusRx.cpp
    HEADERS Oculus/Oculus.h
            RmUtil/RmUtil.h
            Oculus/OsClientCtrl.h
            Oculus/DataWrapper.h
            Oculus/OssDataWrapper.h
            Oculus/OsStatusRx.h
            Oculus/ImageAndRange.hpp
    DEPS_TARGET Qt5::Core Qt5::Network Qt5::Widgets)


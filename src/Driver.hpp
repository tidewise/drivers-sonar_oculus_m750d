#ifndef SONAR_OCULUS_M750D_DRIVER_HPP
#define SONAR_OCULUS_M750D_DRIVER_HPP

#include "iodrivers_base/Driver.hpp"

namespace sonar_oculus_m750d {
    class Driver : iodrivers_base::Driver {
        static const int INTERNAL_BUFFER_SIZE = 200000;

    public:
        Driver();
        int extractPacket(uint8_t const* buffer, size_t buffer_size) const;
    };
}

#endif
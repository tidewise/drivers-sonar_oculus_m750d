#ifndef SONAR_OCULUS_M750D_DRIVER_HPP
#define SONAR_OCULUS_M750D_DRIVER_HPP

#include "Protocol.hpp"
#include "iodrivers_base/Driver.hpp"
#include <base/samples/Sonar.hpp>

namespace sonar_oculus_m750d {
    static const int INTERNAL_BUFFER_SIZE = 200000;
    class Driver : iodrivers_base::Driver {

    public:
        Driver();
        int extractPacket(uint8_t const* buffer, size_t buffer_size) const;
        base::samples::Sonar processOne();
        uint8_t m_read_buffer[INTERNAL_BUFFER_SIZE];
        Protocol m_protocol;
    };
}

#endif
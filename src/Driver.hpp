#ifndef SONAR_OCULUS_M750D_DRIVER_HPP
#define SONAR_OCULUS_M750D_DRIVER_HPP

#include "Protocol.hpp"
#include <base/samples/Sonar.hpp>
#include <iodrivers_base/Driver.hpp>
#include <memory>

namespace sonar_oculus_m750d {
    class Driver : public iodrivers_base::Driver {

    public:
        static const int INTERNAL_BUFFER_SIZE = 200000;

        Driver();
        base::samples::Sonar processOne();
        void fireSonar(int mode,
            double range,
            double gain,
            double speed_of_sound,
            double salinity,
            bool gain_assist,
            uint8_t gamma,
            uint8_t net_speed_limit);
        Protocol m_protocol;

    private:
        virtual int extractPacket(uint8_t const* buffer, size_t buffer_size) const final;
        uint8_t m_read_buffer[INTERNAL_BUFFER_SIZE];
        uint8_t m_write_buffer[INTERNAL_BUFFER_SIZE];
    };
}

#endif // SONAR_OCULUS_M750D_DRIVER_HPP
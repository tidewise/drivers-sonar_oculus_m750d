#ifndef SONAR_OCULUS_M750D_DRIVER_HPP
#define SONAR_OCULUS_M750D_DRIVER_HPP

#include <base/samples/Sonar.hpp>
#include <iodrivers_base/Driver.hpp>
#include <memory>
#include <sonar_oculus_m750d/M750DConfiguration.hpp>
#include <sonar_oculus_m750d/Protocol.hpp>

namespace sonar_oculus_m750d {
    class Driver : public iodrivers_base::Driver {

    public:
        static const int INTERNAL_BUFFER_SIZE = 200000;

        Driver(base::Angle const& beam_width, base::Angle const& beam_height);
        std::optional<base::samples::Sonar> processOne();
        /**
         * @brief It calls a sonar reconfiguration
         *
         * This is also needed to keep the sonar alive
         *
         */
        void fireSonar(M750DConfiguration const& configuration);
        Protocol m_protocol;

    private:
        virtual int extractPacket(uint8_t const* buffer, size_t buffer_size) const final;
        uint8_t m_read_buffer[INTERNAL_BUFFER_SIZE];
        uint8_t m_write_buffer[INTERNAL_BUFFER_SIZE];
        base::Angle m_beam_width;
        base::Angle m_beam_height;
    };
}

#endif // SONAR_OCULUS_M750D_DRIVER_HPP
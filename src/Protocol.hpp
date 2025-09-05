#ifndef SONAR_OCULUS_M750D_PROTOCOL_HPP
#define SONAR_OCULUS_M750D_PROTOCOL_HPP

#include "Oculus.h"
#include "SonarData.hpp"
#include <base/samples/Sonar.hpp>
#include <stdio.h>

namespace sonar_oculus_m750d {
    class Protocol {
    public:
        void handleBuffer(uint8_t const* buffer, size_t buffer_size);
        base::samples::Sonar parseSonar();

        static std::vector<float> toBeamFirst(uint8_t* bin_first,
            uint16_t beam_count,
            uint16_t bin_count);

        static base::Time binDuration(double range, double speed_of_sound, int bin_count);

    private:
        void handleMessagePingResult(uint8_t const* buffer);
        void handleMessageSimplePingResult(uint8_t const* buffer,
            OculusMessageHeader const& header);

        SonarData m_sonar_data;
        bool m_simple = false;

        static constexpr double HORIZONTAL_FOV_DEG = 130;
        static constexpr double VERTICAL_FOV_DEG = 20;
    };
}

#endif // SONAR_OCULUS_M750D_PROTOCOL_HPP
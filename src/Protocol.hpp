#ifndef SONAR_OCULUS_M750D_PROTOCOL_HPP
#define SONAR_OCULUS_M750D_PROTOCOL_HPP

#include "Oculus.h"
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
        OculusSimplePingResult m_simple_ping_result;
        OculusSimplePingResult2 m_simple_ping_result_2;
        OculusReturnFireMessage m_return_fire_message;

        uint8_t* m_image;
        short* m_bearings;
        bool m_simple = false;
        uint16_t m_version;
        static constexpr double HORIZONTAL_FOV_DEG = 130;
        static constexpr double VERTICAL_FOV_DEG = 20;
    };
}

#endif
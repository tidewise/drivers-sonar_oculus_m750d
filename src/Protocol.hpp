#ifndef SONAR_OCULUS_M750D_PROTOCOL_HPP
#define SONAR_OCULUS_M750D_PROTOCOL_HPP

#include "SonarData.hpp"
#include <base/samples/Sonar.hpp>
#include <stdio.h>

namespace sonar_oculus_m750d {
    class Protocol {
    public:
        void handleBuffer(uint8_t const* buffer);
        base::samples::Sonar parseSonar();
        /**
         * @brief Rearrange the sonar data in beam major order
         *
         * Takes an sonar data in bin major order, this is
         * [idx = bin * beam_count + beam], and returns an vector of data arranged
         * in beam major order, this is [idx = beam * bin_count + bin]
         *
         */
        static std::vector<float> toBeamMajor(uint8_t* bin_first,
            uint16_t beam_count,
            uint16_t bin_count);
        static base::Time binDuration(double range, double speed_of_sound, int bin_count);

    private:
        void handleMessageSimplePingResult(uint8_t const* buffer, uint16_t version);

        SonarData m_data;
        bool m_simple_ping_result = false;

        static constexpr double HORIZONTAL_FOV_DEG = 130;
        static constexpr double VERTICAL_FOV_DEG = 20;
    };
}

#endif // SONAR_OCULUS_M750D_PROTOCOL_HPP
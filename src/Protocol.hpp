#ifndef SONAR_OCULUS_M750D_PROTOCOL_HPP
#define SONAR_OCULUS_M750D_PROTOCOL_HPP

#include <base/samples/Sonar.hpp>
#include <sonar_oculus_m750d/SonarData.hpp>
#include <stdio.h>

namespace sonar_oculus_m750d {
    class Protocol {
    public:
        bool handleBuffer(uint8_t const* buffer);
        base::samples::Sonar parseSonar(base::Angle const& beam_width,
            base::Angle const& beam_height,
            bool major_change);
        /**
         * @brief Rearrange the sonar data in beam major order
         *
         * Takes an sonar data in bin major order, this is
         * [idx = bin * beam_count + beam], and returns an vector of data arranged
         * in beam major order, this is [idx = beam * bin_count + bin]
         *
         */
        static std::vector<float> toBeamMajor(std::vector<uint8_t> const& bin_first,
            uint16_t beam_count,
            uint16_t bin_count,
            bool major_change);
        static base::Time binDuration(double range, double speed_of_sound, int bin_count);

    private:
        void handleMessageSimplePingResult(uint8_t const* buffer, uint16_t version);

        SonarData m_data;
        bool m_simple_ping_result = false;
    };
}

#endif // SONAR_OCULUS_M750D_PROTOCOL_HPP
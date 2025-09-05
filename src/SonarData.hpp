#ifndef SONAR_OCULUS_M750D_SONARDATA_HPP
#define SONAR_OCULUS_M750D_SONARDATA_HPP

#include <base/Float.hpp>
#include <cstdint>

namespace sonar_oculus_m750d {
    struct SonarData {
        uint32_t image_size = 0;
        uint32_t image_offset = 0;
        uint16_t beam_count = 0;
        uint16_t bin_count = 0;
        double range = base::unknown<double>();
        double speed_of_sound = base::unknown<double>();
        uint8_t* image;
        short* bearings;
    };
}

#endif // SONAR_OCULUS_M750D_SONARDATA_HPP
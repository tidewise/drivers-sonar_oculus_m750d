#ifndef SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP
#define SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP

#include <base/Float.hpp>
#include <cstdint>

namespace sonar_oculus_m750d {
    struct M750DConfiguration {
        int mode = 0;
        double range = base::unknown<double>();
        double gain = base::unknown<double>();
        double speed_of_sound = base::unknown<double>();
        double salinity = base::unknown<double>();
        bool gain_assist = false;
        uint8_t gamma = 0;
        uint8_t net_speed_limit = 0;
    };
}

#endif // SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP
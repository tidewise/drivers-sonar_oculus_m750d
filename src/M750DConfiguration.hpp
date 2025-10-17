#ifndef SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP
#define SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP

#include <base/Float.hpp>
#include <cstdint>

namespace sonar_oculus_m750d {
    struct M750DConfiguration {
        /**
         * @brief The frequency mode
         *
         * 1 - Low frequency mode: Lowest image definition, maximum range  of 120 meters
         * and frequency of 750kHz
         *
         * 2 - High frequency mode: Highest image definition, maximum range of 40 meters
         * and frequency of 1.2MHz
         *
         */
        int mode = 0;
        /**
         * @brief The range in meters
         *
         */
        double range = base::unknown<double>();
        /**
         * @brief The demanded gain value
         *
         */
        double gain = base::unknown<double>();
        /**
         * @brief The speed of sound in the environment in meters/second
         *
         */
        double speed_of_sound = base::unknown<double>();
        /**
         * @brief The salinity of the environment in parts per thousand (ppt)
         *
         */
        double salinity = base::unknown<double>();
        /**
         * @brief Enables the automatic-gain control
         *
         * When it is enabled, the gain acts like a brightness control
         */
        bool gain_assist = false;
        /**
         * @brief The gamma correction
         *
         * The non-linear dark-to-light mapping of acoustic echo intensities
         */
        uint8_t gamma = 0;
        /**
         * @brief Maximum network speed
         *
         */
        uint8_t net_speed_limit = 0;
    };
}

#endif // SONAR_OCULUS_M750D_M750DCONFIGURATION_HPP
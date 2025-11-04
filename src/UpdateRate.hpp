#ifndef SONAR_OCULUS_M750D_UPDATERATE_HPP
#define SONAR_OCULUS_M750D_UPDATERATE_HPP

#include <stdint.h>

namespace sonar_oculus_m750d {
    enum UpdateRate : uint8_t {
        UPDATE_10HZ_MAX = 0x00, // 10Hz max ping rate
        UPDATE_15HZ_MAX = 0x01, // 15Hz max ping rate
        UPDATE_40HZ_MAX = 0x02, // 40Hz max ping rate
        UPDATE_5HZ_MAX = 0x03,  // 5Hz max ping rate
        UPDATE_2HZ_MAX = 0x04,  // 2Hz max ping rate
        UPDATE_STANDBY = 0x05   // Disable ping
    };
}

#endif
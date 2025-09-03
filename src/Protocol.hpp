#ifndef SONAR_OCULUS_M750D_PROTOCOL_HPP
#define SONAR_OCULUS_M750D_PROTOCOL_HPP

#include "Oculus.h"
#include <stdio.h>

namespace sonar_oculus_m750d {
    class Protocol {
    public:
        void handleBuffer(uint8_t const* buffer, size_t buffer_size);

    private:
        OculusSimplePingResult m_simple_ping_result;
        OculusSimplePingResult2 m_simple_ping_result_2;
        OculusReturnFireMessage m_return_fire_message;

        uint8_t* m_image;
        short* m_bearings;
        bool m_simple = false;
        uint16_t m_version;
    };
}

#endif
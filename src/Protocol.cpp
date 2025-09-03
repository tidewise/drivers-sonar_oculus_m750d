#include <cstdlib>
#include <sonar_oculus_m750d/Protocol.hpp>
#include <string.h>

using namespace sonar_oculus_m750d;

void Protocol::handleBuffer(uint8_t const* buffer, size_t buffer_size)
{
    OculusMessageHeader head;
    memcpy(&head, buffer, sizeof(OculusMessageHeader));
    switch (head.msgId) {
        case messageSimplePingResult: {
            m_simple = true;

            // Get the version of the ping result
            uint32_t imageSize = 0;
            uint32_t image_offset = 0;
            uint16_t beams = 0;
            uint32_t size = 0;

            // Store the version number, this will help us
            m_version = head.msgVersion;

            // Check for V1 or V2 simple ping result
            if (m_version == 2) {
                memcpy(&m_simple_ping_result_2, buffer, sizeof(OculusSimplePingResult2));

                imageSize = m_simple_ping_result_2.imageSize;
                image_offset = m_simple_ping_result_2.imageOffset;
                beams = m_simple_ping_result_2.nBeams;
                size = sizeof(OculusSimplePingResult2);
            }
            else {
                memcpy(&m_simple_ping_result, buffer, sizeof(OculusSimplePingResult));

                imageSize = m_simple_ping_result.imageSize;
                image_offset = m_simple_ping_result.imageOffset;
                beams = m_simple_ping_result.nBeams;
                size = sizeof(OculusSimplePingResult);
            }

            m_image = (uint8_t*)realloc(m_image, imageSize);

            if (m_image)
                memcpy(m_image, buffer + image_offset, imageSize);

            // Copy the bearing table
            m_bearings = (short*)realloc(m_bearings, beams * sizeof(short));

            if (m_bearings) {
                memcpy(m_bearings, buffer + size, beams * sizeof(short));
            }
            break;
        }
        case messagePingResult: {
            m_simple = false;

            memcpy(&m_return_fire_message, buffer, sizeof(OculusReturnFireMessage));
            // Should be safe to copy the image
            m_image =
                (uint8_t*)realloc(m_image, m_return_fire_message.ping_params.imageSize);

            if (m_image) {
                memcpy(m_image,
                    buffer + m_return_fire_message.ping_params.imageOffset,
                    m_return_fire_message.ping_params.imageSize);
            }

            // Copy the bearing table
            m_bearings = (short*)realloc(m_bearings,
                m_return_fire_message.ping.nBeams * sizeof(short));

            if (m_bearings) {
                memcpy(m_bearings,
                    buffer + sizeof(OculusReturnFireMessage),
                    m_return_fire_message.ping.nBeams * sizeof(short));
            }
            break;
        }
    }
}
#include <cstdlib>
#include <sonar_oculus_m750d/Protocol.hpp>
#include <string.h>

using namespace sonar_oculus_m750d;

std::vector<float> Protocol::toBeamFirst(uint8_t* bin_first,
    uint16_t beam_count,
    uint16_t bin_count)
{
    std::vector<float> beam_first(beam_count * bin_count);
    for (uint16_t b = 0; b < beam_count; b++) {
        for (uint16_t r = 0; r < bin_count; r++) {
            beam_first[b * bin_count + r] =
                static_cast<float>(bin_first[r * beam_count + b]);
        }
    }
    return beam_first;
}

base::Time Protocol::binDuration(double range, double speed_of_sound, int bin_count)
{
    return base::Time::fromSeconds(range / (speed_of_sound * bin_count));
}

base::samples::Sonar Protocol::parseSonar()
{
    int beam_count = 0;
    int bin_count = 0;
    double range = 0;
    double speed_of_sound = 0;
    if (m_simple) {
        if (m_version == 2) {
            beam_count = m_simple_ping_result_2.nBeams;
            bin_count = m_simple_ping_result_2.nRanges;
            range = bin_count * m_simple_ping_result_2.rangeResolution;
            speed_of_sound = m_simple_ping_result_2.speedOfSoundUsed;
        }
        else {
            beam_count = m_simple_ping_result.nBeams;
            bin_count = m_simple_ping_result.nRanges;
            range = bin_count * m_simple_ping_result.rangeResolution;
            speed_of_sound = m_simple_ping_result.speedOfSoundUsed;
        }
    }
    else {
        // TODO: I didn't find the speed of sound in this message
        throw std::runtime_error("OculusReturnFireMessage parse is not implemented");
        // beam_count = m_return_fire_message.ping.nBeams;
        // bin_count = m_return_fire_message.ping_params.nRangeLinesBfm;
        // range = m_return_fire_message.ping.range;
        // bins_size = m_return_fire_message.ping_params.imageSize;
    }

    auto bins = toBeamFirst(m_image, beam_count, bin_count);
    auto bin_duration = binDuration(range, speed_of_sound, bin_count);
    auto beam_width = base::Angle::fromDeg(HORIZONTAL_FOV_DEG / beam_count);
    auto beam_height = base::Angle::fromDeg(VERTICAL_FOV_DEG);

    base::samples::Sonar sonar(base::Time::now(),
        bin_duration,
        bin_count,
        beam_width,
        beam_height,
        beam_count,
        false);
    sonar.bins = bins;
    return sonar;
}

void Protocol::handleBuffer(uint8_t const* buffer, size_t buffer_size)
{
    OculusMessageHeader head;
    memcpy(&head, buffer, sizeof(OculusMessageHeader));
    switch (head.msgId) {
        case messageSimplePingResult: {
            m_simple = true;

            uint32_t imageSize = 0;
            uint32_t image_offset = 0;
            uint16_t beams = 0;
            uint32_t size = 0;

            m_version = head.msgVersion;

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

            m_bearings = (short*)realloc(m_bearings, beams * sizeof(short));

            if (m_bearings) {
                memcpy(m_bearings, buffer + size, beams * sizeof(short));
            }
            break;
        }
        case messagePingResult: {
            m_simple = false;

            memcpy(&m_return_fire_message, buffer, sizeof(OculusReturnFireMessage));
            m_image =
                (uint8_t*)realloc(m_image, m_return_fire_message.ping_params.imageSize);

            if (m_image) {
                memcpy(m_image,
                    buffer + m_return_fire_message.ping_params.imageOffset,
                    m_return_fire_message.ping_params.imageSize);
            }

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
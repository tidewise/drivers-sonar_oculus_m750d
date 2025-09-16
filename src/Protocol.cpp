#include "Oculus.h"
#include <cstdlib>
#include <sonar_oculus_m750d/Protocol.hpp>
#include <string.h>

using namespace sonar_oculus_m750d;

void Protocol::handleBuffer(uint8_t const* buffer)
{
    OculusMessageHeader header;
    memcpy(&header, buffer, sizeof(OculusMessageHeader));
    switch (header.msgId) {
        case messageSimplePingResult:
            handleMessageSimplePingResult(buffer, header.msgVersion);
            break;
        case messagePingResult:
            throw std::runtime_error("messagePingResult handler is not implemented");
            break;
        default:
            break;
    }
}

void setBearings(SonarData& sonar_data, uint32_t size, uint8_t const* buffer);
void setImage(SonarData& sonar_data, uint32_t image_offset, uint8_t const* buffer);

void Protocol::handleMessageSimplePingResult(uint8_t const* buffer, uint16_t version)
{
    m_simple_ping_result = true;

    uint32_t size = 0;
    uint32_t image_offset = 0;

    if (version == 2) {
        OculusSimplePingResult2 result;
        size = sizeof(OculusSimplePingResult2);
        memcpy(&result, buffer, size);
        m_data.image_size = result.imageSize;
        m_data.beam_count = result.nBeams;
        m_data.bin_count = result.nRanges;
        m_data.range = m_data.bin_count * result.rangeResolution;
        m_data.speed_of_sound = result.speedOfSoundUsed;
        image_offset = result.imageOffset;
    }
    else {
        OculusSimplePingResult result;
        size = sizeof(OculusSimplePingResult);
        memcpy(&result, buffer, size);
        m_data.image_size = result.imageSize;
        m_data.beam_count = result.nBeams;
        m_data.bin_count = result.nRanges;
        m_data.range = m_data.bin_count * result.rangeResolution;
        m_data.speed_of_sound = result.speedOfSoundUsed;
        image_offset = result.imageOffset;
    }

    setImage(m_data, image_offset, buffer);
    setBearings(m_data, size, buffer);
}

void setImage(SonarData& sonar_data, uint32_t image_offset, uint8_t const* buffer)
{
    sonar_data.image = (uint8_t*)realloc(sonar_data.image, sonar_data.image_size);
    if (sonar_data.image) {
        memcpy(sonar_data.image, buffer + image_offset, sonar_data.image_size);
    }
}

void setBearings(SonarData& sonar_data, uint32_t size, uint8_t const* buffer)
{
    sonar_data.bearings =
        (short*)realloc(sonar_data.bearings, sonar_data.beam_count * sizeof(short));
    if (sonar_data.bearings) {
        memcpy(sonar_data.bearings, buffer + size, sonar_data.beam_count * sizeof(short));
    }
}

std::vector<base::Angle> getBearingsAngles(short* bearings, uint16_t beam_count);

base::samples::Sonar Protocol::parseSonar()
{
    if (!m_simple_ping_result) {
        throw std::runtime_error("OculusReturnFireMessage parse is not implemented");
    }

    auto bin_duration =
        binDuration(m_data.range, m_data.speed_of_sound, m_data.bin_count);
    auto beam_width = base::Angle::fromDeg(HORIZONTAL_FOV_DEG / m_data.beam_count);
    auto beam_height = base::Angle::fromDeg(VERTICAL_FOV_DEG);
    base::samples::Sonar sonar(base::Time::now(),
        bin_duration,
        m_data.bin_count,
        beam_width,
        beam_height,
        m_data.beam_count,
        false);
    auto bins = toBeamMajor(m_data.image, m_data.beam_count, m_data.bin_count);
    sonar.bins = bins;
    sonar.bearings = getBearingsAngles(m_data.bearings, m_data.beam_count);

    return sonar;
}

base::Time Protocol::binDuration(double range, double speed_of_sound, int bin_count)
{
    return base::Time::fromSeconds(range / (speed_of_sound * bin_count));
}

std::vector<float> Protocol::toBeamMajor(uint8_t* bin_first,
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

std::vector<base::Angle> getBearingsAngles(short* bearings, uint16_t beam_count)
{
    std::vector<base::Angle> bearings_angles;
    for (size_t i = 0; i < beam_count; i++) {
        bearings_angles[i] = base::Angle::fromDeg(static_cast<double>(bearings[i]) / 100);
    }
    return bearings_angles;
}
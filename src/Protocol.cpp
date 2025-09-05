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
    if (!m_simple) {
        throw std::runtime_error("OculusReturnFireMessage parse is not implemented");
    }

    auto bin_duration = binDuration(m_sonar_data.range,
        m_sonar_data.speed_of_sound,
        m_sonar_data.bin_count);
    auto beam_width = base::Angle::fromDeg(HORIZONTAL_FOV_DEG / m_sonar_data.beam_count);
    auto beam_height = base::Angle::fromDeg(VERTICAL_FOV_DEG);
    base::samples::Sonar sonar(base::Time::now(),
        bin_duration,
        m_sonar_data.bin_count,
        beam_width,
        beam_height,
        m_sonar_data.beam_count,
        false);
    auto bins =
        toBeamFirst(m_sonar_data.image, m_sonar_data.beam_count, m_sonar_data.bin_count);
    sonar.bins = bins;

    // TODO
    // sonar.setRegularBeamBearings(start, interval);

    return sonar;
}

void Protocol::handleBuffer(uint8_t const* buffer, size_t buffer_size)
{
    OculusMessageHeader header;
    memcpy(&header, buffer, sizeof(OculusMessageHeader));
    switch (header.msgId) {
        case messageSimplePingResult:
            handleMessageSimplePingResult(buffer, header);
            break;
        case messagePingResult:
            handleMessagePingResult(buffer);
            break;
        default:
            break;
    }
}

void Protocol::handleMessagePingResult(uint8_t const* buffer)
{
    m_simple = false;

    OculusReturnFireMessage return_fire_message;
    auto msg_size = sizeof(OculusReturnFireMessage);
    memcpy(&return_fire_message, buffer, msg_size);

    auto ping = return_fire_message.ping;
    auto ping_params = return_fire_message.ping_params;
    m_sonar_data.image = (uint8_t*)realloc(m_sonar_data.image, ping_params.imageSize);

    if (m_sonar_data.image) {
        memcpy(m_sonar_data.image,
            buffer + ping_params.imageOffset,
            ping_params.imageSize);
    }

    m_sonar_data.bearings =
        (short*)realloc(m_sonar_data.bearings, ping.nBeams * sizeof(short));

    if (m_sonar_data.bearings) {
        memcpy(m_sonar_data.bearings, buffer + msg_size, ping.nBeams * sizeof(short));
    }

    m_sonar_data.beam_count = ping.nBeams;
    m_sonar_data.bin_count = ping_params.nRangeLinesBfm;
    m_sonar_data.range = ping.range;
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

void Protocol::handleMessageSimplePingResult(uint8_t const* buffer,
    OculusMessageHeader const& header)
{
    m_simple = true;

    uint32_t size = 0;
    uint32_t image_offset = 0;

    if (header.msgVersion == 2) {
        OculusSimplePingResult2 result;
        size = sizeof(OculusSimplePingResult2);
        memcpy(&result, buffer, size);
        m_sonar_data.image_size = result.imageSize;
        image_offset = result.imageOffset;
        m_sonar_data.beam_count = result.nBeams;
        m_sonar_data.bin_count = result.nRanges;
        m_sonar_data.range = m_sonar_data.bin_count * result.rangeResolution;
        m_sonar_data.speed_of_sound = result.speedOfSoundUsed;
    }
    else {
        OculusSimplePingResult result;
        size = sizeof(OculusSimplePingResult);
        memcpy(&result, buffer, size);
        m_sonar_data.image_size = result.imageSize;
        image_offset = result.imageOffset;
        m_sonar_data.beam_count = result.nBeams;
        m_sonar_data.bin_count = result.nRanges;
        m_sonar_data.range = m_sonar_data.bin_count * result.rangeResolution;
        m_sonar_data.speed_of_sound = result.speedOfSoundUsed;
    }

    setImage(m_sonar_data, image_offset, buffer);
    setBearings(m_sonar_data, size, buffer);
}

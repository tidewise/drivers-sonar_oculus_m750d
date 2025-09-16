#include "Driver.hpp"
#include "Oculus.h"
#include <string.h>

using namespace sonar_oculus_m750d;

Driver::Driver(base::Angle const& beam_width, base::Angle const& beam_height)
    : iodrivers_base::Driver::Driver(INTERNAL_BUFFER_SIZE)
    , m_beam_width(beam_width)
    , m_beam_height(beam_height)
{
    m_protocol = Protocol();
}

bool isValidHeader(OculusMessageHeader const& header)
{
    return header.oculusId == OCULUS_CHECK_ID;
}

int Driver::extractPacket(uint8_t const* buffer, size_t buffer_size) const
{
    auto header_size = sizeof(OculusMessageHeader);

    if (buffer_size < header_size) {
        // Not enough data for a header — wait for more
        return 0;
    }

    OculusMessageHeader header;
    memcpy(&header, buffer, header_size);
    if (!isValidHeader(header)) {
        // discard first byte
        return -1;
    }

    size_t packet_size = header_size + header.payloadSize;
    if (buffer_size >= packet_size) {
        // Packet starting at zero is valid
        return packet_size;
    }

    // Not enough bytes for a valid packet - wait for new bytes
    return 0;
}

std::optional<base::samples::Sonar> Driver::processOne()
{
    readPacket(m_read_buffer, INTERNAL_BUFFER_SIZE);
    if (m_protocol.handleBuffer(m_read_buffer)) {
        auto sonar = m_protocol.parseSonar(m_beam_width, m_beam_height);
        return sonar;
    }
    return std::nullopt;
}

uint8_t setFlags(bool gain_assist);

void Driver::fireSonar(M750DConfiguration const& config)
{
    OculusSimpleFireMessage simple_fire_message;
    memset(&simple_fire_message, 0, sizeof(OculusSimpleFireMessage));
    simple_fire_message.head.msgId = messageSimpleFire;
    simple_fire_message.head.srcDeviceId = 0;
    simple_fire_message.head.dstDeviceId = 0;
    simple_fire_message.head.oculusId = 0x4f53;
    uint8_t flags = setFlags(config.gain_assist);
    simple_fire_message.flags = flags;
    simple_fire_message.gammaCorrection = config.gamma;
    simple_fire_message.pingRate = pingRateHigh;
    simple_fire_message.networkSpeed = config.net_speed_limit;
    simple_fire_message.masterMode = config.mode;
    simple_fire_message.range = config.range;
    simple_fire_message.gainPercent = config.gain;
    simple_fire_message.speedOfSound = config.speed_of_sound;
    simple_fire_message.salinity = config.salinity;

    writePacket(reinterpret_cast<uint8_t*>(&simple_fire_message),
        sizeof(OculusSimpleFireMessage));
}

uint8_t setFlags(bool gain_assist)
{
    // Range in metres
    uint8_t flags = 0x01;

    if (gain_assist) {
        // Enable gain assist
        flags |= 0x10;
    }

    // Oculus will output simple fire returns
    flags |= 0x08;

    // Enable 512 beams
    flags |= 0x40;

    return flags;
}

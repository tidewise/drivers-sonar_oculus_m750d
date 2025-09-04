#include "Driver.hpp"
#include "Oculus.h"
#include <string.h>

using namespace sonar_oculus_m750d;

Driver::Driver()
    : iodrivers_base::Driver::Driver(INTERNAL_BUFFER_SIZE)
{
    m_protocol = Protocol();
}

int Driver::extractPacket(uint8_t const* buffer, size_t buffer_size) const
{
    if (buffer_size < sizeof(OculusMessageHeader)) {
        // Not enough data for a header — wait for more
        return 0;
    }

    OculusMessageHeader header;
    memcpy(&header, buffer, sizeof(OculusMessageHeader));
    if (header.oculusId != OCULUS_CHECK_ID) {
        // Invalid data in the header - // discard first byte
        return -1;
    }

    size_t packet_size = sizeof(OculusMessageHeader) + header.payloadSize;
    if (buffer_size >= packet_size) {
        // Packet starting at zero is valid
        return packet_size;
    }

    // Not enough bytes for a valid packet - wait for new bytes
    return 0;
}

base::samples::Sonar Driver::processOne()
{
    int packet_size = readPacket(m_read_buffer, INTERNAL_BUFFER_SIZE);
    m_protocol.handleBuffer(m_read_buffer, packet_size);
    auto sonar = m_protocol.parseSonar();
    return sonar;
}

void Driver::fireSonar(int mode,
    double range,
    double gain,
    double speed_of_sound,
    double salinity,
    bool gain_assist,
    uint8_t gamma,
    uint8_t net_speed_limit)
{
    OculusSimpleFireMessage simple_fire_message;
    memset(&simple_fire_message, 0, sizeof(OculusSimpleFireMessage));
    simple_fire_message.head.msgId = messageSimpleFire;
    simple_fire_message.head.srcDeviceId = 0;
    simple_fire_message.head.dstDeviceId = 0;
    simple_fire_message.head.oculusId = 0x4f53;

    // Range in metres
    uint8_t flags = 0x01;

    if (gain_assist) {
        // flagsGainAssist
        flags |= 0x10;
    }

    // Oculus will output simple fire returns
    flags |= 0x08;

    // Enable 512 beams
    flags |= 0x40;

    simple_fire_message.flags = flags;
    simple_fire_message.gammaCorrection = gamma;
    simple_fire_message.pingRate = pingRateHigh;
    simple_fire_message.networkSpeed = net_speed_limit;
    simple_fire_message.masterMode = mode;
    simple_fire_message.range = range;
    simple_fire_message.gainPercent = gain;
    simple_fire_message.speedOfSound = speed_of_sound;
    simple_fire_message.salinity = salinity;

    writePacket((uint8_t*)&simple_fire_message, sizeof(OculusSimpleFireMessage));
}

#include "Driver.hpp"
#include "Oculus.h"
#include <string.h>

using namespace sonar_oculus_m750d;

Driver::Driver()
    : iodrivers_base::Driver::Driver(INTERNAL_BUFFER_SIZE)
{
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
        // Invalid data in the header - flush the buffer
        return -1;
    }

    size_t packet_size = sizeof(OculusMessageHeader) + header.payloadSize;
    if (buffer_size >= packet_size)
    {
        // Packet starting at zero is valid
        return packet_size;
    }

    // Not enough bytes for a valid packet - wait for new bytes
    return 0;
}
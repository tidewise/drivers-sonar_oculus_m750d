#ifndef SONAR_DATA_HPP
#define SONAR_DATA_HPP

namespace sonar_oculus_m750d {
    struct SonarData
    {
        short* bearings;
        uint16_t beam_count;
        uint16_t bin_count;
        double range;
        uchar* data;
        uint32_t data_size;
        double speed_of_sound;
    };
    
}

#endif
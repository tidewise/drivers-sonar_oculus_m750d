#ifndef IMAGE_HPP
#define IMAGE_HPP

namespace sonar_oculus_m750d {
    struct ImageAndRange {
        uchar* data;
        size_t data_size;
        size_t height;
        size_t width;
        double range;
    };
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
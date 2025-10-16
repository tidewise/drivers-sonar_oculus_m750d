#include <gtest/gtest.h>
#include <sonar_oculus_m750d/Protocol.hpp>

#include <iostream>

using namespace sonar_oculus_m750d;
using namespace std;

struct ProtocolTest : public ::testing::Test {
    Protocol protocol = Protocol();
};

TEST_F(ProtocolTest, it_changes_the_bins_to_beam_major)
{
    std::vector<uint8_t> bins = {1, 2, 3, 4, 5, 6};
    uint16_t beam_count = 3;
    uint16_t bin_count = 2;
    auto beam_first = protocol.toBeamMajor(bins, beam_count, bin_count);
    std::vector<uint8_t> expected_beam_first = {1, 4, 2, 5, 3, 6};
    ASSERT_EQ(expected_beam_first, beam_first);
}

TEST_F(ProtocolTest, it_calculates_the_bin_duration)
{
    double range = 100;
    double speed_of_sound = 1000;
    int bin_count = 200;
    auto bin_duration = protocol.binDuration(range, speed_of_sound, bin_count);
    auto expected_bin_duration = base::Time::fromSeconds(5e-4);
    ASSERT_EQ(expected_bin_duration, bin_duration);
}

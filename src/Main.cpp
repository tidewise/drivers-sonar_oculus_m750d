#include <iostream>
#include <sonar_oculus_m750d/Driver.hpp>
#include <sonar_oculus_m750d/Protocol.hpp>

using namespace std;
using namespace sonar_oculus_m750d;

int usage()
{
    cerr << "Usage: "
         << "sonar_oculus_m750d_ctl URI \n"
         << "URI is a valid iodrivers_base URI, e.g. tcp://192.168.1.200:52100\n"
         << flush;
    return 0;
}
int main(int argc, char const* argv[])
{
    if (argc < 2) {
        cerr << "not enough arguments" << endl;
        return usage();
    }
    string uri(argv[1]);
    base::Angle beam_width = base::Angle::fromDeg(0.25390625);
    base::Angle beam_height = base::Angle::fromDeg(20);
    bool major_change = true;
    Driver driver(beam_width, beam_height, major_change);
    driver.setReadTimeout(base::Time::fromMilliseconds(2000));
    driver.setWriteTimeout(base::Time::fromMilliseconds(1000));
    driver.openURI(uri);
    M750DConfiguration conf;
    conf.mode = 1;
    conf.gain = 1;
    conf.gain_assist = false;
    conf.gamma = 1;
    conf.net_speed_limit = 255;
    conf.range = 120;
    conf.salinity = 35;
    conf.speed_of_sound = 1500;
    driver.fireSonar(conf);
    while (true) {
        auto sonar = driver.processOne();
        if (sonar) {
            driver.fireSonar(conf);
            std::cout << "bins size = " << sonar->bins.size() << std::endl;
        }
    }
}

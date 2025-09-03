#include <sonar_oculus_m750d/Driver.hpp>

using namespace sonar_oculus_m750d;

Driver::Driver()
    : iodrivers_base::Driver::Driver(INTERNAL_BUFFER_SIZE)
{
}
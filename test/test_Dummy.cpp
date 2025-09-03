#include <boost/test/unit_test.hpp>
#include <sonar_oculus_m750d/Dummy.hpp>

using namespace sonar_oculus_m750d;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    sonar_oculus_m750d::DummyClass dummy;
    dummy.welcome();
}

#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_ScriptingServiceReturnValuesHolder"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

#include "Arguments.hpp"

namespace aw {
namespace core {
namespace scripting {
namespace testing {

namespace {

class SomeClass {};
typedef boost::shared_ptr<SomeClass> SomeClassPtr;

const boost::any anyInt32 =       int32_t();
const boost::any anyFloat =       float();
const boost::any anyString =      std::string();
const boost::any anyCustomClass = SomeClassPtr();

} // namespace anonymous


BOOST_AUTO_TEST_CASE(TestHolderNoContent) {

    ReturnValuesHolder holder = ReturnValuesHolder::Create();
    BOOST_CHECK_EQUAL(holder.Size(), 0);
    BOOST_CHECK_THROW(holder.SetValue(0, anyInt32), std::out_of_range);
    BOOST_CHECK_THROW(holder.GetValue(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(TestHolderDifferentTypes) {

    ReturnValuesHolder holder = ReturnValuesHolder::Create<int32_t, float, std::string>();
    BOOST_REQUIRE_EQUAL(holder.Size(), 3);

    ReturnValue firstReturn, secondReturn, thirdReturn;
    BOOST_REQUIRE_NO_THROW(firstReturn = holder.GetValue(0));
    BOOST_REQUIRE_NO_THROW(secondReturn = holder.GetValue(1));
    BOOST_REQUIRE_NO_THROW(thirdReturn = holder.GetValue(2));

    // Check the types returned:
    BOOST_CHECK(firstReturn.type() == anyInt32.type());
    BOOST_CHECK(secondReturn.type() == anyFloat.type());
    BOOST_CHECK(thirdReturn.type() == anyString.type());

    // Negative test:
    BOOST_CHECK(firstReturn.type() != anyCustomClass.type());
    BOOST_CHECK(secondReturn.type() != anyCustomClass.type());
    BOOST_CHECK(thirdReturn.type() != anyCustomClass.type());
}

BOOST_AUTO_TEST_CASE(TestHolderSettingData) {

    ReturnValuesHolder holder = ReturnValuesHolder::Create<int32_t>();

    // Store an int32
    holder.SetValue(0, ReturnValue(int32_t(42)));
    int32_t storedValue = 0;
    BOOST_CHECK_NO_THROW(storedValue = boost::any_cast<int32_t>(holder.GetValue(0)));
    BOOST_CHECK_EQUAL(storedValue, 42);

    // Try storing another type.
    BOOST_CHECK_THROW(holder.SetValue(0, ReturnValue(std::string("foo"))), std::runtime_error);
}

} // namespace testing
} // namespace scripting
} // namespace core
} // namespace aw

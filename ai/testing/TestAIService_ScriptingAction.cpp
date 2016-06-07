#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "Test_AIScriptingAction"

#include <boost/test/unit_test.hpp>

#include "IScriptAction.hpp"
#include "InitScriptingService.hpp"

#include "IScriptingService.hpp"

#include "AIServiceFixture.hpp"

#include <boost/intrusive_ptr.hpp>

namespace aw {
namespace core {
namespace ai {
namespace testing {

struct TestFixture : public AIServiceFixture {

    TestFixture() {

        BOOST_REQUIRE_NO_THROW(scripting::RegisterService(serviceLocator));

        // Check if service is available...
        base::IServicePtr service = base::IServicePtr();
        BOOST_REQUIRE_NO_THROW(service = serviceLocator->GetService(scripting::ID_SCRIPTING_SERVICE));
        BOOST_REQUIRE(service);

        // ...and the right one...
        scriptingService = boost::dynamic_pointer_cast<scripting::IScriptingService>(service);
        BOOST_REQUIRE(scriptingService);
    }

    scripting::IScriptingServicePtr scriptingService;
};

// Todo: Write tests for:
//          Invalid file.
//          Unresolvable file, i.e. no resolver registered to the ScriptingService.
//          Delaved / immediate resolving of file.



/*
BOOST_FIXTURE_TEST_CASE(TestEmptyContainer, AIBasicsFixture) {

    BOOST_CHECK_EQUAL(taskContainer->GetNumTasks(), 0);
}
*/


} // namespace testing
} // namespace ai
} // namespace core
} // namespace aw
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE "MyTestSuite"

#include <iostream>
#include <boost/test/unit_test.hpp>

#include "Action.hpp"

BOOST_AUTO_TEST_CASE(CreatingAnAction) {


    std::cout << "Foo" << std::endl;
}
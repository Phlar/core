#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test_ReferenceCounted"

#pragma warning (disable: 4250)

#include <IReferenceCounted.hpp>
#include <ReferenceCounted.hpp>
#include <InterfaceImpl.hpp>

#include <boost/intrusive_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/noncopyable.hpp>

#include <iostream>


namespace aw {
namespace core {
namespace base {

class ISomeClass : public virtual IReferenceCounted {

    public:

        virtual int GetReferenceCount() const = 0;
}; 

class SomeClass : public ISomeClass, public base::ReferenceCounted, public boost::noncopyable
{    
    public:

        SomeClass(bool& alive)
        : isAlive(alive) {

            isAlive = true;
        }

        ~SomeClass() {

            isAlive = false;
        }

        int GetReferenceCount() const {

            return ReferenceCounted::GetRefCount();
        }

        bool& isAlive;
};

BOOST_AUTO_TEST_CASE(TestRefCountedInstances) {

    typedef boost::intrusive_ptr<ISomeClass> ISomeClassPtr;
    bool alive = false;

    {
        ISomeClassPtr instanceA;
        {
            ISomeClassPtr instanceB;

            BOOST_REQUIRE(!instanceA);
            BOOST_REQUIRE(!instanceB);

            // Simple instantiation should result in a ref-count of "1"
            instanceA = ISomeClassPtr(new SomeClass(alive));
            BOOST_CHECK(alive);
            BOOST_CHECK_EQUAL(instanceA->GetReferenceCount(), 1);

            // Assign instance to another pointer
            instanceB = instanceA;
            BOOST_CHECK(alive);
            BOOST_CHECK_EQUAL(instanceB, instanceA);
            BOOST_CHECK_EQUAL(instanceB->GetReferenceCount(), 2);

            // Assign via copy constructor
            ISomeClassPtr instanceC(instanceB);
            BOOST_CHECK(alive);
            BOOST_CHECK_EQUAL(instanceC, instanceA);
            BOOST_CHECK_EQUAL(instanceC->GetReferenceCount(), 3);
        }

        // Instance "C" and "B" do not exist any longer - however "A" still is
        // alive - thus keeping the ref-count t "1" and the underlying object alive.
        BOOST_CHECK(alive);
        BOOST_CHECK_EQUAL(instanceA->GetReferenceCount(), 1);
    }
    
}

} // namespace base
} // namespace core
} // namespace aw

#pragma warning (default: 4250)
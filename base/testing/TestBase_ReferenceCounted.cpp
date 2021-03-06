#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "Test_ReferenceCounted"

#include <IReferenceCounted.hpp>
#include <ReferenceCounted.hpp>
#include <InterfaceImpl.hpp>

#include <boost/intrusive_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/noncopyable.hpp>

#include <iostream>


#pragma warning (disable: 4250)

namespace aw {
namespace core {
namespace base {
namespace testing {

class ISomeClass : public virtual IReferenceCounted {

    public:

        virtual int GetReferenceCount() const = 0;
}; 

class SomeClass : public InterfaceImpl<ISomeClass>, public boost::noncopyable
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

BOOST_AUTO_TEST_CASE(TestWrapIntoMultipleIntrusivePointers) {

    typedef boost::intrusive_ptr<ISomeClass> ISomeClassPtr;

    bool alive = false;

    {
        ISomeClassPtr intrusivePointerA;
        {
            intrusivePointerA = ISomeClassPtr(new SomeClass(alive));
            BOOST_CHECK(alive);
            BOOST_CHECK_EQUAL(intrusivePointerA->GetReferenceCount(), 1);

            // Set up the second intrusive-pointer, referring to the same content.
            ISomeClassPtr intrusivePointerB;
            intrusivePointerB = ISomeClassPtr(intrusivePointerA.get());
            BOOST_CHECK(alive);
            BOOST_CHECK_EQUAL(intrusivePointerA->GetReferenceCount(), 2);
            BOOST_CHECK_EQUAL(intrusivePointerB->GetReferenceCount(), 2);
            BOOST_CHECK_EQUAL(intrusivePointerA.get(), intrusivePointerB.get());
        }

        BOOST_CHECK(alive);
        BOOST_CHECK_EQUAL(intrusivePointerA->GetReferenceCount(), 1);
    }
    BOOST_CHECK(!alive);
}

} // namespace testing
} // namespace base
} // namespace core
} // namespace aw

#pragma warning (default: 4250)
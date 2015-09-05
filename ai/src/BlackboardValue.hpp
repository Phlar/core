#pragma once

#include "IBlackboardValue.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/uuid/random_generator.hpp>


namespace aw {
namespace core {
namespace ai {
namespace support {


//! \brief Helper class implementing IBlackboardValue taking care of
//! providing a random UUID for the ID attribute.
//! Cannot be instantiated directly in order to enforce users deriving
//! from it to provide a valid UUID for the type.
template<class T>
class BlackboardValue : public base::InterfaceImpl<IBlackboardValue> {

    public:

        BlackboardValue(const T& value, const base::UUID& typeID)
        : m_value(value)
        , m_typeID(typeID) {

            boost::uuids::basic_random_generator<boost::mt19937> uuidGenerator;
            m_id = uuidGenerator();
        }

        //!@{
        //! Implemented from IBlackboardValue
        base::UUID GetTypeID() const override {
          
            return m_typeID;
        };

        base::UUID GetID() const override {

            return m_id;
        }
        //!@}

        const T& GetValue() const {

            return m_value;
        }

        T& GetValue() {

            return m_value;
        }

    protected:
        
        virtual ~BlackboardValue() {
        }

        //! Enforce using not a default c'tor
        BlackboardValue();

        base::UUID m_id;
        base::UUID m_typeID;
        T    m_value;
};

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
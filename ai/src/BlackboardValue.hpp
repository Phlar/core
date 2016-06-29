#pragma once

#include "IBlackboardValue.hpp"

#include "UUID.hpp"
#include "InterfaceImpl.hpp"

#include <boost/uuid/random_generator.hpp>

#include <sstream>

namespace aw {
namespace core {
namespace ai {
namespace support {


//! \brief Implementing the IBlackboardValue interface taking care of
//! providing a random UUID for the ID attribute.
//! Cannot be instantiated directly in order to enforce users deriving
//! from it to provide a valid UUID for the type.
template<class T>
class BlackboardValue : public base::InterfaceImpl<IBlackboardValue> {

    public:

        BlackboardValue(const base::UUID& typeID, const T& value = T())
        : m_value(value)
        , m_semanticTypeID(typeID) {

            boost::uuids::basic_random_generator<boost::mt19937> uuidGenerator;
            m_id = uuidGenerator();
        }

        //!@{
        //! Implemented from IBlackboardValue
        virtual base::UUID GetSemanticTypeID() const override {
            return m_semanticTypeID;
        };

        virtual base::UUID GetID() const override {
            return m_id;
        }

        virtual void AssignValue(const boost::any& val) {

            const T* rawVal = boost::any_cast<T>(&val);
            if(!rawVal) {
                std::stringstream errorMessage;
                errorMessage << "Error assigning value to blackboard-value, cast failed from '"
                             << val.type().name() << "' to '"
                             << typeid(T).name() << "'";
                throw std::runtime_error(errorMessage.str());
            }
            m_value = *rawVal;
        }

        virtual boost::any GetValue() const {
            return boost::any(m_value);
        }
        //!@}

        void operator=(const T& rhs) {

            m_value = rhs;
        }

    protected:
        
        virtual ~BlackboardValue() {
        }

        //! Enforce using not a default c'tor
        BlackboardValue();

        base::UUID m_id;
        base::UUID m_semanticTypeID;
        T          m_value;
};

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
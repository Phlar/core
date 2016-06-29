#pragma once

#include "AIFwdDeclarations.hpp"

#include "UUID.hpp"

#include "IBlackboard.hpp"
#include "BlackboardValue.hpp"

#include <boost/intrusive_ptr.hpp>

#include <sstream>


namespace aw {
namespace core {
namespace ai {
namespace support {

// BlackboardValue support functions.
template<typename T, const base::UUID& SID>
struct SemanticTypeTrait {
    typedef T type;
    static const base::UUID semanticID;
};

template<typename T, const base::UUID& SID>
const base::UUID SemanticTypeTrait<T, SID>::semanticID = SID;

// Creator utilizing the type-trait above.
template<typename T>
IBlackboardValuePtr createBlackboardValue(typename T::type val = T::type())
{
    return createBlackboardValue<T::type>(T::semanticID, T::type(val));
}

// Plain creator function.
template<typename T>
IBlackboardValuePtr createBlackboardValue(const base::UUID& semanticID, typename T val = T())
{
    return boost::intrusive_ptr<BlackboardValue<T> >
        (new BlackboardValue<T>(semanticID, val));
}

// Retrieve the raw type from the blackboard-value.
template<typename T>
T getRawValue(IBlackboardValuePtr blackBoardValue) {

    if(!blackBoardValue) {
        throw std::invalid_argument("Invalid blackboard-value to retrieve value from.");
    }

    boost::any anyVal = blackBoardValue->GetValue();
    const T* ptrVal = boost::any_cast<T>(&anyVal);
    if(!ptrVal) {

        std::stringstream errorMessage;
        errorMessage << "Error fetching 'raw' blackboard-value, cast failed from '"
            << anyVal.type().name() << "' to '"
            << typeid(T).name() << "'";
        throw std::runtime_error(errorMessage.str());
    }
    return *ptrVal;
}

// Blackboard support functions.

IBlackboardValuePtr getValueFromBlackboard(IBlackboardPtr blackboard, const base::UUID& semanticID);

template<typename T>
T getRawValueFromBlackboard(IBlackboardPtr blackboard, const base::UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    IBlackboardValuePtr blackBoardValue = blackboard->GetValue(semanticID);
    if(!blackBoardValue) {
        throw std::logic_error("No value listed on black-board matching the semantic type-ID.");
    }

    return getRawValue<T>(blackBoardValue);
}

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw


//! \brief Convenience macro easing the definition of blackboard-value types.
#define BLACKBOARD_TYPE(NAME, TYPE, UUID)                                           \
    typedef support::SemanticTypeTrait<TYPE, UUID> NAME;                            \
    typedef support::BlackboardValue<NAME::type> Blackboard##NAME##Value;           \
    typedef boost::intrusive_ptr<Blackboard##NAME##Value> NAME##Ptr;

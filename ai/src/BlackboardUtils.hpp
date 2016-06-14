#pragma once

#include "AIFwdDeclarations.hpp"

#include "UUID.hpp"

#include "IBlackboard.hpp"
#include "BlackboardValue.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>

namespace aw {
namespace core {
namespace ai {
namespace support {


template<typename T, const base::UUID& SID>
struct SemanticTypeTrait {
    typedef T type;
    static const base::UUID semanticID;
};

template<typename T, const base::UUID& SID>
const base::UUID SemanticTypeTrait<T, SID>::semanticID = SID;


template<typename T>
boost::intrusive_ptr<support::BlackboardValue<typename T::type>>
createBlackBoardValue(typename T::type defaultValue = T::type())
{
    return boost::intrusive_ptr<BlackboardValue<T::type> >
        (new BlackboardValue<T::type>(defaultValue, T::semanticID));
}

IBlackboardValuePtr getValue(IBlackboardPtr blackboard, const base::UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    return blackboard->GetValue(semanticID);
}

template<typename T>
T& castBlackboardValueToType(IBlackboardValuePtr blackBoardValue) {

    if(!blackBoardValue) {
        throw std::invalid_argument("Invalid blackboard-value to retrieve value from.");
    }

    auto castedValue = boost::dynamic_pointer_cast<BlackboardValue<T>>(blackBoardValue);
    if(!castedValue) {
        throw std::logic_error("Cannot cast blackboard value to target type.");
    }
    return castedValue->GetValue();
}

template<typename T>
T& getTypedValue(IBlackboardPtr blackboard, const base::UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    IBlackboardValuePtr blackBoardValue = blackboard->GetValue(semanticID);
    if(!blackBoardValue) {
        throw std::logic_error("No value listed on black-board matching the semantic type-ID.");
    }

    return castBlackboardValueToType<T>(blackBoardValue);
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

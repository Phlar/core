#pragma once

#include "AIFwdDeclarations.hpp"

#include "IBlackboard.hpp"
#include "BlackboardValue.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/uuid/string_generator.hpp>

namespace aw {
namespace core {
namespace ai {
namespace support {


template<typename T, const UUID& SID>
struct SemanticTypeTrait {
    typedef T type;
    static const UUID semanticID;
};

template<typename T, const UUID& SID>
const UUID SemanticTypeTrait<T, SID>::semanticID = SID;


template<typename T>
boost::intrusive_ptr<support::BlackboardValue<typename T::type>>
    createBlackBoardValue(typename T::type defaultValue = T::type())
{
    return boost::intrusive_ptr<BlackboardValue<T::type> >
        (new BlackboardValue<T::type>(defaultValue, T::semanticID));
}

template<typename T>
const T& requireSingleBlackBoardValue(IBlackboardPtr blackboard, const UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    auto values = blackboard->GetValuesByType(semanticID);
    if(values.size() != 1) {
        throw std::logic_error("Expected exactly one value.");
    }

    auto castedValue = boost::dynamic_pointer_cast<BlackboardValue<T>>(values.front());
    if(!castedValue) ??<
        throw std::logic_error("Cannot cast blackboard value to target type.");
    }

    return castedValue->GetValue();
}

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
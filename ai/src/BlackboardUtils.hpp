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

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
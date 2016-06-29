#include "BlackboardUtils.hpp"

#include <Utils.hpp>

#include <iostream>

namespace aw {
namespace core {
namespace ai {
namespace support {

IBlackboardValuePtr getValueFromBlackboard(IBlackboardPtr blackboard, const base::UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    return blackboard->GetValue(semanticID);
}

} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
#include "BlackboardUtils.hpp"

#include <Utils.hpp>

#include <iostream>

namespace aw {
namespace core {
namespace ai {
namespace support {

IBlackboardValuePtr getValue(IBlackboardPtr blackboard, const base::UUID& semanticID) {

    if(!blackboard) {
        throw std::invalid_argument("Invalid blackboard to read value from.");
    }

    return blackboard->GetValue(semanticID);
}

IBlackboardValuePtr createBlackBoardValue_uint8(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<uint8_t>(semanticID);
}

IBlackboardValuePtr createBlackBoardValue_uint16(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<uint16_t>(semanticID);
}

IBlackboardValuePtr createBlackBoardValue_uint32(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<uint32_t>(semanticID); 
}

IBlackboardValuePtr createBlackBoardValue_int8(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<int8_t>(semanticID);
}

IBlackboardValuePtr createBlackBoardValue_int16(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<int16_t>(semanticID); 
}

IBlackboardValuePtr createBlackBoardValue_int32(const aw::core::base::UUID& semanticID) {
    return createBlackBoardValue<int32_t>(semanticID);
}

IBlackboardValuePtr createBlackBoardValue_string(const aw::core::base::UUID& semanticID, const std::string& value) {
    return createBlackBoardValue<std::string>(semanticID, value);
}


} // namespace support
} // namespace ai
} // namespace core
} // namespace aw
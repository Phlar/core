#include "Blackboard.hpp"

#include "IBlackboardValue.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


Blackboard::Blackboard()
: m_valueMap() {
}

Blackboard::~Blackboard() {
}

void Blackboard::SetValue(IBlackboardValuePtr value) {

    if(!value) {
        throw std::invalid_argument("Empty value cannot be added to blackboard.");
    }

    if(value->GetTypeID().is_nil()) {
        throw std::invalid_argument("Value having invalid type-ID cannot be added to blackboard");
    }

    if(value->GetID().is_nil()) {
        throw std::invalid_argument("Value having invalid ID cannot be added to blackboard");
    }

    m_valueMap[value->GetTypeID()] = value;
}

IBlackboardValuePtr Blackboard::GetValue(const base::UUID& semanticID) const {

    ValueMap::const_iterator findIter = m_valueMap.find(semanticID);
    if(findIter != m_valueMap.end()) {
        return findIter->second;
    }

    return IBlackboardValuePtr();
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
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

void Blackboard::AddValue(IBlackboardValuePtr value) {

    if(!value) {
        throw std::invalid_argument("Empty value cannot be added to blackboard.");
    }

    if(value->GetTypeID().is_nil()) {
        throw std::invalid_argument("Value having invalid type-ID cannot be added to blackboard");
    }

    if(value->GetID().is_nil()) {
        throw std::invalid_argument("Value having invalid ID cannot be added to blackboard");
    }

    // Insert or retrieve list of values
    std::pair<ValueMap::iterator, bool> insertResult = 
        m_valueMap.insert(std::make_pair(value->GetTypeID(), BlackboardValueList(1, value)));

    // There's already elements listed with the type-ID...
    if(!insertResult.second)
    {
        // ...check whether the value with the ID is already in the container
        BlackboardValueList& valueContainer = (insertResult.first)->second;
        if(std::find_if(valueContainer.begin(), valueContainer.end(), 
            [&value](const IBlackboardValuePtr& findValue) -> bool 
            {
                return findValue->GetID() == value->GetID();
            }) != valueContainer.end()) {

            throw std::logic_error("Value already listed in blackboard.");

        } else {

            valueContainer.push_back(value);
        }
    }  
}

BlackboardValueList Blackboard::GetValuesByType(const UUID& typeID) const {

    ValueMap::const_iterator findIter = m_valueMap.find(typeID);
    if(findIter != m_valueMap.end()) {
        return findIter->second;
    }

    return BlackboardValueList();
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
#include "Blackboard.hpp"

#include "IBlackboardValue.hpp"
#include "ITaskParameter.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


Blackboard::Blackboard()
: m_valueMap()
, m_taskParameters() {
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

void Blackboard::StoreTaskParameter(ITaskParameterPtr taskParameter) {

    if(!taskParameter) {
        throw(std::invalid_argument("Empty task-parameter cannot be added to blackboard"));
    }

    if(taskParameter->GetAssociatedTaskID().is_nil()) {
        throw std::invalid_argument(
            "Task-parameter having invalid ID as the referenced task's ID cannot be added to blackboard");
    }

    m_taskParameters[taskParameter->GetAssociatedTaskID()] = taskParameter;
}

ITaskParameterPtr Blackboard::GetTaskParameter(const UUID& taskID) const {

    TaskParameterMap::const_iterator iter = m_taskParameters.find(taskID);
    if(iter != m_taskParameters.end()) {
        return iter->second;
    }

    return ITaskParameterPtr();
}

bool Blackboard::RemoveTaskParameter(const UUID& taskID) {

    return m_taskParameters.erase(taskID) != 0 ? true : false;
}

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
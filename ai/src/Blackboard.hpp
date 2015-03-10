#pragma once

#include "AIFwdDeclarations.hpp"
#include "IBlackboard.hpp"

#include "InterfaceImpl.hpp"

#include <map>


namespace aw {
namespace core {
namespace ai {
namespace impl {


class Blackboard : public base::InterfaceImpl<IBlackboard> {

    public:

        Blackboard();
        virtual ~Blackboard();

        //@{
        //! Implementations of IBlackboard
        void AddValue(IBlackboardValuePtr value);
        BlackboardValueList GetValuesByType(const UUID& typeID) const;
        void StoreTaskParameter(ITaskParameterPtr taskParameter);
        ITaskParameterPtr GetTaskParameter(const UUID& taskID);
        bool RemoveTaskParameter(const UUID& taskID);
        //@}

    protected:

        //! Use a map from type-ID to a ordered list of values.
        typedef std::map<UUID, BlackboardValueList> ValueMap;
        ValueMap m_valueMap;

        //! Mapping from the associated task ID to the task-parameter.
        //! \todo: Think of a set with a custom look-up functor- removing
        //! redundancy in storing task-IDs.
        typedef std::map<UUID, ITaskParameterPtr> TaskParameterMap;
        TaskParameterMap m_taskParameters;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
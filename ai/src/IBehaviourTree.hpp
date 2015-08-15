#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"
#include "ITask.hpp"

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


enum class BehaviourTreeState {

    STATE_NOT_RUN = 0,
    STATE_RUNNING,
    STATE_FINISHED
};

class IBehaviourTree : public virtual base::IReferenceCounted {

    public:

        //! \brief Set the topmost root the tree is composed of.
        virtual void SetRoot(ITaskPtr rootTask) = 0;

        //! \brief Perform a stepwise execution of the tree.
        virtual BehaviourTreeState EvaluateOneTick() = 0;
};

typedef boost::intrusive_ptr<IBehaviourTree> IBehaviourTreePtr;

} // namespace ai
} // namespace core
} // namespace aw
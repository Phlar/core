#pragma once

#include "AIFwdDeclarations.hpp"

#include "IReferenceCounted.hpp"
#include "ITask.hpp"

#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {


class IBehaviourTree : public virtual base::IReferenceCounted {

    public:

        virtual void SetTreeRoot(ITaskPtr rootTask) = 0;

        virtual TaskResult EvaluateOnce() = 0;
};

typedef boost::intrusive_ptr<IBehaviourTree> IBehaviourTreePtr;

} // namespace ai
} // namespace core
} // namespace aw
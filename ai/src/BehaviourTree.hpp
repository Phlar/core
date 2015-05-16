#pragma once

#include "AIFwdDeclarations.hpp"
#include "IBehaviourTree.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {
namespace impl {


class BehaviourTree : public base::InterfaceImpl<IBehaviourTree> {

    public:

        BehaviourTree();
        virtual ~BehaviourTree();

        //@{
        //! Implementations of IBehviourTree
        void SetTreeRoot(ITaskPtr rootTask);
        TaskResult EvaluateOnce();
        //@}

    protected:

};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
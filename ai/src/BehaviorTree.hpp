#pragma once

#pragma warning (disable: 4512)

#include "AIFwdDeclarations.hpp"
#include "IBehaviorTree.hpp"

#include "InterfaceImpl.hpp"


namespace aw {
namespace core {
namespace ai {


class BehaviorTree : public base::InterfaceImpl<IBehaviorTree> {

    public:

        BehaviorTree(ITaskPtr task = ITaskPtr());
        virtual ~BehaviorTree();

        //! Implementations from IBehaviorTree
        void SetRoot(ITaskPtr task);
        void SetProcessingType(TreeProcessingType processingType);
        void Process();

    protected:

        ITaskPtr m_rootTask;
        TreeProcessingType m_processingType;
};

} // namespace ai
} // namespace core
} // namespace aw

#pragma warning (default: 4512)

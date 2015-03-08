#pragma once

#include "IReferenceCounted.hpp"
#include "AIFwdDeclarations.hpp"

#include <boost/signals2.hpp>

namespace aw {
namespace core {
namespace ai {


typedef boost::signals2::signal<void()> SignalTreeProcessed;


enum class TreeProcessingType {

    PROCESS_ONE_CYCLE = 0,
    PROCESS_ONE_STEP,
    PROCESS_LOOP
};

class IBehaviorTree : public base::IReferenceCounted {

    public:

        //! \brief Defines the root of a behavior subtree.
        void SetRoot(ITaskPtr task);

        //! \brief Define how to process the tree.
        void SetProcessingType(TreeProcessingType treeProcessingType);

        //! \brief Processes the tree from the current task onwards.
        //! In case of the entire tree was processed - ending up back at
        //! the root, sigTreeProcessed will be emitted.
        void Process();

        //! \brief Emitted if one cycle of the entire tree was finished.
        SignalTreeProcessed signalTreeProcessed;                    
};

} // namespace ai
} // namespace core
} // namespace aw
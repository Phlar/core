#pragma once

#include "ITask.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {

//! \brief Condition that executes a script-snippet.
class IScriptCondition : public virtual ITask {

    public:

        //! \brief Define the path of the file to invoke when executing this condition-task.
        //! \param filePath the location of the file itself.
        //! \param functionName name of the function to invoke. The signature of the function must be int(IBlackboardPtr).
        //! \param delayLoad delay the loading itself until the action gets executed the first time. 
        virtual void SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) = 0;

};
typedef boost::intrusive_ptr<IScriptCondition> IScriptConditionPtr;

} // namespace ai
} // namespace core
} // namespace aw
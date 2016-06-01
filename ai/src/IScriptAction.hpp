#pragma once

#include "AIFwdDeclarations.hpp"
#include "IAction.hpp"

#include "IReferenceCounted.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/intrusive_ptr.hpp>


namespace aw {
namespace core {
namespace ai {

//! \brief Specialized action that executes a script-snippet.
class IScriptAction : public virtual IAction {

    public:

        //! \brief Define the path of the file to invoke when executing this action-task.
        //! \param filePath the location of the file itself.
        //! \param functionName name of the function to invoke. The signature of the function must be int(IBlackboardPtr).
        //! \param delayLoad delay the loading itself until the action gets executed the first time. 
        virtual void SetScriptFile(const boost::filesystem::path& filePath, const std::string& functionName, bool delayLoad) = 0;

};
typedef boost::intrusive_ptr<IScriptAction> IScriptActionPtr;

} // namespace ai
} // namespace core
} // namespace aw
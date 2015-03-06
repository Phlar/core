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
        //@}

    protected:

        //! Use a map from type-ID to a ordered list of values.
        typedef std::map<UUID, BlackboardValueList> ValueMap;
        ValueMap m_valueMap;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
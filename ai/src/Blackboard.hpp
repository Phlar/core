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
        void SetValue(IBlackboardValuePtr value) override;
        IBlackboardValuePtr GetValue(const UUID& semanticID) const override;
        //@}

    protected:

        //! Use a map from a semantic type-ID to a value.
        typedef std::map<UUID, IBlackboardValuePtr> ValueMap;
        ValueMap m_valueMap;
};

} // namespace impl
} // namespace ai
} // namespace core
} // namespace aw
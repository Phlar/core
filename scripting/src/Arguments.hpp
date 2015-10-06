#pragma once

#include "../../ai/src/IBlackboard.hpp"

#include <boost/variant.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {

// Variant of arguments that get passed to script resolvers as function parameters.
typedef boost::variant<bool,
                       int16_t,
                       float,
                       std::string, 
                       aw::core::ai::IBlackboardPtr> Argument;
typedef std::vector<Argument> ArgumentVector;

} // namespace scripting
} // namespace core
} // namespace aw




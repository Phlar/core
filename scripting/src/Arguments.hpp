#pragma once

#include "../../ai/src/IBlackboard.hpp"

#include <boost/any.hpp>

#include <vector>

namespace aw {
namespace core {
namespace scripting {

// Variant of arguments that get passed to script resolvers as function parameters.
typedef boost::any Argument;
typedef std::vector<Argument> ArgumentVector;

} // namespace scripting
} // namespace core
} // namespace aw

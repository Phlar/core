#pragma once

#include "IRepeater.hpp"

#include <boost/noncopyable.hpp>

namespace aw {
namespace core {
namespace ai {


class RepeatConditionCounter : public RepeatCondition,
                               public boost::noncopyable {

    public:

        RepeatConditionCounter(uint16_t numLoops)
        : m_numLoops(numLoops)
        , m_runLoops(0)
        {}
        
        virtual ~RepeatConditionCounter() {};

        bool Evaluate() {

            const bool belowMaxLoops = (m_runLoops < m_numLoops);
            if(belowMaxLoops) {
                ++m_runLoops;
            }

            return belowMaxLoops;
        }

        void ResetCondition() {

            m_runLoops = 0;
        }

    protected:

        const uint16_t m_numLoops;
        uint16_t m_runLoops;
};


} // namespace ai
} // namespace core
} // namespace aw
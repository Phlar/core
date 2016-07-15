#pragma once

#include <boost/atomic.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>


namespace aw {
namespace core {
namespace base {

class Timer {

    public:

        typedef boost::function<void()> Callback;
        typedef boost::shared_ptr<boost::thread> ThreadPtr;

        Timer();
        virtual ~Timer();

        ThreadPtr Start(uint32_t milliseconds, const Callback& callback);
        void Pause();
        void Resume();

    protected:

        enum class TimerState {

            TIMER_FINISHED = 0,
            TIMER_RUNNING,
            TIMER_PAUSED
        };

        boost::atomic<boost::chrono::steady_clock::time_point> m_timerStart;
        boost::atomic<boost::chrono::steady_clock::time_point> m_expectedTimerEnd;

        boost::atomic<TimerState> m_timerState;
        ThreadPtr m_timerThread;
        Callback m_callback;
};

} // namespace base
} // namespace core
} // namespace
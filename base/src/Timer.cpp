#include "Timer.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace aw {
namespace core {
namespace base {

#include <iostream>

Timer::Timer()
: m_timerStart()
, m_expectedTimerEnd()
, m_timerState(TimerState::TIMER_FINISHED)
, m_timerThread()
, m_callback() {
}

Timer::~Timer() {
}

Timer::ThreadPtr Timer::Start(uint32_t milliseconds, const Callback& callback) {

    assert(!m_timerThread);

    m_callback = callback;

    if(m_timerState.load() == TimerState::TIMER_RUNNING ||
       m_timerState.load() == TimerState::TIMER_PAUSED) {
            std::cout << "Cannot start a running / paused timer" << std::endl;
    }

    m_timerThread = ThreadPtr(new boost::thread([this, milliseconds]() {

        this->m_timerStart.store(boost::chrono::steady_clock::now());
        this->m_expectedTimerEnd.store(this->m_timerStart.load() + boost::chrono::milliseconds(milliseconds));
        // std::cout << "Timer started at " << m_timerStart.load() << ", expected end at " << m_expectedTimerEnd.load() << std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(milliseconds));
        this->m_callback();
    }));

    return m_timerThread;
}

void Timer::Pause() {
}

void Timer::Resume() {

}

} // namespace base
} // namespace core
} // namespace
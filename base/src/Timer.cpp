#include "Timer.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace aw {
namespace core {
namespace base {

#include <iostream>

Timer::Timer()
: m_expectedTimerEnd()
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

        std::cout << "Starting timer." << std::endl;
        m_timerState.store(TimerState::TIMER_RUNNING);

        this->m_expectedTimerEnd.store(boost::chrono::steady_clock::now() + boost::chrono::milliseconds(milliseconds));
        m_timerStart = boost::chrono::steady_clock::now();

        while(m_timerState.load() == TimerState::TIMER_RUNNING || m_timerState.load() == TimerState::TIMER_PAUSED) {

            const auto sleepingTime = this->m_expectedTimerEnd.load() - boost::chrono::steady_clock::now();
            std::cout << "Sleeping for " << sleepingTime.count() * 0.000001 << "ms." << std::endl;

            boost::this_thread::sleep(boost::posix_time::milliseconds(static_cast<int64_t>(sleepingTime.count() * 0.000001)));
            std::cout << "Finished sleeping." << std::endl;

            if(m_timerState.load() == TimerState::TIMER_PAUSED) {
                std::cout << "Timer is paused, waiting for resume being called." << std::endl;

                boost::unique_lock<boost::mutex> lock(m_syncMutex);
                m_syncVariable.wait(lock);
            }

            if(boost::chrono::steady_clock::now() > this->m_expectedTimerEnd.load()) {
                m_timerState.store(TimerState::TIMER_FINISHED);
            }
        };

        std::cout << "Overall sleeping time: " << (boost::chrono::steady_clock::now() - m_timerStart).count() * 0.000001 << "ms." << std::endl;

        this->m_callback();
    }));

    return m_timerThread;
}

void Timer::Pause() {

    std::cout << "Issuing pause." << std::endl;

    assert(m_timerState.load() == TimerState::TIMER_RUNNING);
    assert(boost::chrono::steady_clock::now() < m_expectedTimerEnd.load());
    
    m_timerState.store(TimerState::TIMER_PAUSED);
    m_pauseStarted = boost::chrono::steady_clock::now();
}

void Timer::Resume() {

    std::cout << "Issuing resume." << std::endl;

    assert(m_timerState.load() == TimerState::TIMER_PAUSED);
    // auto sleepRemainder = m_expectedTimerEnd.load() - m_pauseStarted.load();
    auto pausedTime = boost::chrono::steady_clock::now() - m_pauseStarted.load();
    auto newExpectedTimerEnd = m_expectedTimerEnd.load() + pausedTime;
    m_expectedTimerEnd.store(newExpectedTimerEnd);

    m_timerState.store(TimerState::TIMER_RUNNING);

    m_syncVariable.notify_all();
}

} // namespace base
} // namespace core
} // namespace
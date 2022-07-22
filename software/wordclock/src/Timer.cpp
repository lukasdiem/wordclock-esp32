#include <Arduino.h>
#include "Timer.h"
#include "logging.h"
#include "mutex.h"

namespace
{

    mutex_t timerMux;
    volatile int interruptCounter = 0;

    void IRAM_ATTR onTimer_()
    {
        GetMutex(&timerMux);
        interruptCounter++;
        ReleaseMutex(&timerMux);
    }

} // namespace

Timer::Timer() {
    // Create the mutex
    CreateMutex(&timerMux);
}

void Timer::setup(std::function<void()> callback, uint32_t delay_seconds)
{
    callback_ = callback;
    delay_seconds_ = delay_seconds;
}

void Timer::start()
{
    timer1_attachInterrupt(&onTimer_);
    // 80 MHz / 16 => 5MHz (0.2us)
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    // 2us * 5000 => 1 kHz (1ms)
    timer1_write(5000);
}

// start/stop the timer
void Timer::stop()
{
    timer1_disable();
    timer1_detachInterrupt();
}

void Timer::loop()
{
    if (interruptCounter > 0)
    {
        getMutex(10);
        
        interruptCounter--;
        ReleaseMutex(&timerMux);

        if (callback_ != NULL)
        {
            callback_();
        }
    }
}

bool Timer::getMutex(int retries)
{
    bool result = GetMutex(&timerMux);
    // get the mutex
    int mutexCounter = 0;
    while (result == false)
    {
        if (mutexCounter > retries)
        {
            DLOG("Mutex not released within ");
            DLOG(retries);
            DLOGLN(" attempts");
            break;
        }
        mutexCounter++;
        result = GetMutex(&timerMux);
    }

    return result;
}
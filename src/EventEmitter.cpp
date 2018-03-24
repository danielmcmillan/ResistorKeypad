#include "EventEmitter.h"
#include <thread>

namespace ResistorKeypad
{
EventEmitter::EventEmitter(Reader reader, std::chrono::milliseconds refreshPeriod)
    : reader(reader), refreshPeriod(refreshPeriod)
{
    thread = std::thread(&EventEmitter::threadWorker, this, std::move(stopPromise.get_future()));
}

EventEmitter::~EventEmitter()
{
    stopPromise.set_value();
    thread.join();
}

EventEmitter::ListenerID EventEmitter::addListener(const Listener listener)
{
    std::lock_guard<std::mutex> lock(mutex);
    ListenerID id = ++lastId;
    listeners[id] = listener;
}

void EventEmitter::removeListener(ListenerID listenerID)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto listenerIt = listeners.find(listenerID);
    listeners.erase(listenerIt);
}

void EventEmitter::threadWorker(std::future<void> stopFuture)
{
    while (stopFuture.wait_for(refreshPeriod) == std::future_status::timeout)
    {
        int button;
        if (reader.buttonPressed(&button))
        {
            if (!isButtonPressed || button != lastPressedButton)
            {
                isButtonPressed = true;
                lastPressedButton = button;
                emit({button});
            }
        }
        else
        {
            isButtonPressed = false;
        }
    }
}

void EventEmitter::emit(ButtonPressedEvent event)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (auto &listener : listeners)
    {
        listener.second(event);
    }
}
}
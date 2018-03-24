#include "Events.h"
#include <thread>

namespace Keypad
{
Events::Events(std::chrono::milliseconds refreshPeriod)
    : refreshPeriod(refreshPeriod)
{
    thread = std::thread(&Events::threadWorker, this, std::move(stopPromise.get_future()));
}

Events::~Events()
{
    stopPromise.set_value();
    thread.join();
}

Events::ListenerID Events::addListener(const Listener listener)
{
    std::lock_guard<std::mutex> lock(mutex);
    ListenerID id = ++lastId;
    listeners[id] = listener;
}

void Events::removeListener(ListenerID listenerID)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto listenerIt = listeners.find(listenerID);
    listeners.erase(listenerIt);
}

void Events::threadWorker(std::future<void> stopFuture)
{
    while (stopFuture.wait_for(refreshPeriod) == std::future_status::timeout)
    {
        emit({5});
    }
}

void Events::emit(ButtonPressedEvent event)
{
    std::lock_guard<std::mutex> lock(mutex);
    for (auto &listener : listeners)
    {
        listener.second(event);
    }
}
}
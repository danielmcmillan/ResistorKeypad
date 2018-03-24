#include "Events.h"
#include <chrono>
#include <thread>

namespace Keypad
{
Events::Events()// : thread(stopPromise.get_future())
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
    while (stopFuture.wait_for(std::chrono::milliseconds(100)) == std::future_status::timeout)
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
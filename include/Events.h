#pragma once

#include "Keypad.h"
#include <functional>
#include <unordered_map>
#include <mutex>
#include <future>
#include <chrono>

namespace Keypad
{
    const std::chrono::milliseconds defaultRefreshPeriod = std::chrono::milliseconds(100);
    /**
     * Class for monitoring the Keypad asynchronously and alerting listeners of button presses.
     */
    class Events
    {
    public:
        struct ButtonPressedEvent
        {
            int buttonId;
        };
        using Listener = std::function<void (ButtonPressedEvent)>;
        using ListenerID = int;

        Events(std::chrono::milliseconds refreshPeriod = std::chrono::milliseconds(defaultRefreshPeriod));
        ~Events();

        /**
         * @brief Provide a function to be called when an event occurs.
         * @param listener The function, accepts ButtonPressedEvent as an argument.
         * @return Value for identifier the listener.
         */
        ListenerID addListener(Listener listener);

        /**
         * Stop sending events to the listener with the specified ID.
         * @param listener ID of the listener to remove.
         */
        void removeListener(ListenerID listener);

    private:
        Keypad keypad;
        std::chrono::milliseconds refreshPeriod;
        std::unordered_map<ListenerID, Listener> listeners;
        ListenerID lastId = 0;
        std::mutex mutex;
        std::promise<void> stopPromise;
        std::thread thread;

        void threadWorker(std::future<void> stopFuture);
        void emit(ButtonPressedEvent event);
    };
}
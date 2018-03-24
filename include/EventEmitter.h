#pragma once

#include "Reader.h"
#include <functional>
#include <unordered_map>
#include <mutex>
#include <future>
#include <chrono>

namespace ResistorKeypad
{
    const std::chrono::milliseconds defaultRefreshPeriod = std::chrono::milliseconds(100);
    /**
     * Class for reading from the keypad asynchronously and alerting listeners of button presses.
     */
    class EventEmitter
    {
    public:
        struct ButtonPressedEvent
        {
            int buttonId;
        };
        using Listener = std::function<void (ButtonPressedEvent)>;
        using ListenerID = int;

        EventEmitter(
            Reader reader,
            std::chrono::milliseconds refreshPeriod = std::chrono::milliseconds(defaultRefreshPeriod)
        );
        ~EventEmitter();

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
        Reader reader;
        std::chrono::milliseconds refreshPeriod;
        std::unordered_map<ListenerID, Listener> listeners;
        bool isButtonPressed = false;
        int lastPressedButton;
        ListenerID lastId = 0;
        std::mutex mutex;
        std::promise<void> stopPromise;
        std::thread thread;

        void threadWorker(std::future<void> stopFuture);
        void emit(ButtonPressedEvent event);
    };
}
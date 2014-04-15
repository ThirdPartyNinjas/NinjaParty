#ifndef NINJAPARTY_EVENT_HPP
#define NINJAPARTY_EVENT_HPP

#include <memory>
#include <stdint.h>

namespace NinjaParty
{
    typedef uint32_t EventId;
    
    struct IEvent
    {
    public:
        virtual EventId GetId() = 0;
        
    protected:
        static EventId nextId;
    };
    
    template <class T>
    class Event : public IEvent
    {
    public:
        virtual ~Event()
        {
        }
        
        static EventId GetEventId()
        {
            static EventId eventId = ++nextId;
            return eventId;
        }
        
        EventId GetId()
        {
            return GetEventId();
        }
    };
    
    enum class EventPriority
    {
        Normal = 0,
        High,
        Immediate,
    };

    class IEventReceiver
    {
    public:
        IEventReceiver();
        virtual ~IEventReceiver();
        
        virtual void Receive(const std::shared_ptr<IEvent> &event) = 0;
        
    private:
        friend class EventBroadcaster;
        
        bool needsCleanup;
    };
    
    class EventBroadcaster
    {
    public:
        static EventBroadcaster& Instance();
        
        void AddEventReceiver(const EventId &eventId, IEventReceiver *eventReceiver);
        void RemoveEventReceiver(const EventId &eventId, IEventReceiver *eventReceiver);
        void RemoveEventReceiver(IEventReceiver *eventReceiver);
        
        void Broadcast(const std::shared_ptr<IEvent> &event);
        
        void Execute();
        
    private:
        EventBroadcaster();
        
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
}
#endif//NINJAPARTY_EVENT_HPP

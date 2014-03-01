#ifndef NINJAPARTY_EVENT_HPP
#define NINJAPARTY_EVENT_HPP

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
    
    template <class Derived>
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
    };
}
#endif//NINJAPARTY_EVENT_HPP
#ifndef NINJAPARTY_TOUCHEVENTS_HPP
#define NINJAPARTY_TOUCHEVENTS_HPP

#include "Event.hpp"

namespace NinjaParty
{
    typedef void* TouchHandle;
    
    class TouchBeganEvent : public Event<TouchBeganEvent>
    {
    public:
        TouchBeganEvent(TouchHandle touchHandle, int x, int y, int tapCount)
        : touchHandle(touchHandle), x(x), y(y), tapCount(tapCount)
        {
        }
        
        TouchHandle Handle() const { return touchHandle; }
        int X() const { return x; }
        int Y() const { return y; }
        int TapCount() const { return tapCount; }
        
    private:
        TouchHandle touchHandle;
        int x, y;
        int tapCount;
    };

    class TouchEndedEvent : public Event<TouchEndedEvent>
    {
    public:
        TouchEndedEvent(TouchHandle touchHandle, int x, int y)
        : touchHandle(touchHandle), x(x), y(y)
        {
        }
        
        TouchHandle Handle() const { return touchHandle; }
        int X() const { return x; }
        int Y() const { return y; }
        
    private:
        TouchHandle touchHandle;
        int x, y;
    };
    
    class TouchMovedEvent : public Event<TouchMovedEvent>
    {
    public:
        TouchMovedEvent(TouchHandle touchHandle, int x, int y)
        : touchHandle(touchHandle), x(x), y(y)
        {
        }
        
        TouchHandle Handle() const { return touchHandle; }
        int X() const { return x; }
        int Y() const { return y; }
        
    private:
        TouchHandle touchHandle;
        int x, y;
    };
    
    class TouchCancelledEvent : public Event<TouchCancelledEvent>
    {
    public:
        TouchCancelledEvent(TouchHandle touchHandle)
        : touchHandle(touchHandle)
        {
        }
        
        TouchHandle Handle() const { return touchHandle; }
        
    private:
        TouchHandle touchHandle;
    };
}

#endif//NINJAPARTY_TOUCHEVENTS_HPP

#ifndef NINJAPARTY_GESTUREEVENTS_HPP
#define NINJAPARTY_GESTUREEVENTS_HPP

#include "Event.hpp"

namespace NinjaParty
{
    class GesturePanEvent : public Event<GesturePanEvent>
    {
    public:
        GesturePanEvent(int x, int y)
        : x(x), y(y)
        {
        }
        
        int X() const { return x; }
        int Y() const { return y; }
        
    private:
        int x, y;
    };

    class GesturePinchEvent : public Event<GesturePinchEvent>
    {
    public:
        GesturePinchEvent(int x, int y, float scale)
        : x(x), y(y), scale(scale)
        {
        }
        
        int X() const { return x; }
        int Y() const { return y; }
        float Scale() const { return scale; }
        
    private:
        int x, y;
        float scale;
    };
    
    class GestureRotationEvent : public Event<GestureRotationEvent>
    {
    public:
        GestureRotationEvent(float radians)
        : radians(radians)
        {
        }
        
        float Radians() const { return radians; }
        
    private:
        float radians;
    };

    enum class SwipeDirection
    {
        Up, Down, Left, Right,
    };
    
    class GestureSwipeEvent : public Event<GestureSwipeEvent>
    {
    public:
        GestureSwipeEvent(SwipeDirection direction)
        : direction(direction)
        {
        }

        SwipeDirection Direction() const { return direction; }
        
    private:
        SwipeDirection direction;
    };
    
    class GestureTapEvent : public Event<GestureTapEvent>
    {
    public:
        GestureTapEvent(int x, int y, int touches)
        : x(x), y(y), touches(touches)
        {
        }
        
        int X() const { return x; }
        int Y() const { return y; }
        int Touches() const { return touches; }
        
    private:
        int x, y;
        int touches;
    };
}

#endif//NINJAPARTY_GESTUREEVENTS_HPP

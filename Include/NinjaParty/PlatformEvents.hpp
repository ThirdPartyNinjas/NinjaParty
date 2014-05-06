#ifndef NINJAPARTY_PLATFORMEVENTS_HPP
#define NINJAPARTY_PLATFORMEVENTS_HPP

#include "Event.hpp"

namespace NinjaParty
{
    enum class AudioInterruptionEventType
    {
        Begin,
        End,
    };
    
    class AudioInterruptionEvent : public Event<AudioInterruptionEvent>
    {
    public:
        AudioInterruptionEvent(AudioInterruptionEventType type)
        : type(type)
        {
        }
        
        AudioInterruptionEventType Type() const { return type; }
        
    private:
        AudioInterruptionEventType type;
    };
}

#endif//NINJAPARTY_PLATFORMEVENTS_HPP

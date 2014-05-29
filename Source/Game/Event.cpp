#include <algorithm>
#include <map>
#include <list>
#include <vector>

#include <NinjaParty/Event.hpp>

namespace NinjaParty
{
    EventId IEvent::nextId = 0;
    
    struct EventBroadcaster::impl
    {
        std::map<EventId, std::vector<IEventReceiver*>> eventReceivers;
        
        std::list<std::shared_ptr<IEvent>> events;
    };
    
    IEventReceiver::IEventReceiver()
        : needsCleanup(false)
    {
    }
    
    IEventReceiver::~IEventReceiver()
    {
        if(needsCleanup)
            EventBroadcaster::Instance().RemoveEventReceiver(this);
    }
    
    EventBroadcaster::EventBroadcaster()
        : pimpl(new impl())
    {
    }

    EventBroadcaster& EventBroadcaster::Instance()
    {
        static EventBroadcaster instance;
        return instance;
    }
    
    void EventBroadcaster::AddEventReceiver(const EventId &eventId, IEventReceiver *eventReceiver)
    {
        eventReceiver->needsCleanup = true;

        if(pimpl->eventReceivers.find(eventId) == pimpl->eventReceivers.end())
            pimpl->eventReceivers[eventId] = std::vector<IEventReceiver*>();
        
        auto &v = pimpl->eventReceivers[eventId];
        auto p = std::find(v.begin(), v.end(), eventReceiver);
        
        if(p != v.end())
            *p = eventReceiver;
        else
            v.push_back(eventReceiver);
    }
    
    void EventBroadcaster::RemoveEventReceiver(const EventId &eventId, IEventReceiver *eventReceiver)
    {
        eventReceiver->needsCleanup = false;

        if(pimpl->eventReceivers.find(eventId) == pimpl->eventReceivers.end())
            return;
        
        auto &v = pimpl->eventReceivers[eventId];
        auto p = std::find(v.begin(), v.end(), eventReceiver);
        
        if(p != v.end())
            v.erase(p);
	}

    void EventBroadcaster::RemoveEventReceiver(IEventReceiver *eventReceiver)
    {
        eventReceiver->needsCleanup = false;

        for(auto &kvp : pimpl->eventReceivers)
        {
            auto &v = kvp.second;
            auto p = std::find(v.begin(), v.end(), eventReceiver);
            
            if(p != v.end())
                v.erase(p);
        }
	}
    
    void EventBroadcaster::Broadcast(const std::shared_ptr<IEvent> &event)
    {
        pimpl->events.push_back(event);
    }
    
    void EventBroadcaster::Execute()
    {
        std::list<std::shared_ptr<IEvent>> temporaryList(pimpl->events);
        pimpl->events.clear();
        
        for(auto &event : temporaryList)
        {
            if(pimpl->eventReceivers.find(event->GetId()) == pimpl->eventReceivers.end())
                continue;
            
            for(auto p : pimpl->eventReceivers[event->GetId()])
            {
                p->Receive(event);
            }
        }
    }
}

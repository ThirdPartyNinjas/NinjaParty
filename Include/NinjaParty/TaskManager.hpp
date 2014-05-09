#ifndef NINJAPARTY_TASKMANAGER_HPP
#define NINJAPARTY_TASKMANAGER_HPP

#include <functional>
#include <queue>

#include "Juggler.hpp"

namespace NinjaParty
{
    typedef std::function<void ()> Action;
    typedef std::function<bool ()> Condition;
    typedef std::function<bool (float)> TimedCondition;

    class TaskQueue : public Jugglable
    {
    public:
        TaskQueue() : ended(false) { }
        
        bool IsComplete() const;
        
        void Update(float deltaSeconds);
        
        TaskQueue& Perform(const Action &action);
        TaskQueue& Wait(float seconds);
        TaskQueue& WaitFor(const Condition &condition);
        TaskQueue& WaitUntil(const TimedCondition &timedCondition);
        
        void End();
        
    private:
        enum class TaskType
        {
            Action,
            Condition,
            TimedCondition,
        };
        
        struct Task
        {
            Task(Action action);
            Task(Condition condition);
            Task(TimedCondition timedCondition);
            Task(const Task &task);
            ~Task();
            
            TaskType taskType;
            Action action;
            Condition condition;
            TimedCondition timedCondition;
        };
        
        std::queue<Task> tasks;
        bool ended;
    };
}

#endif//NINJAPARTY_TASKMANAGER_HPP

#ifndef NINJAPARTY_TASKMANAGER_HPP
#define NINJAPARTY_TASKMANAGER_HPP

#include <functional>
#include <queue>

namespace NinjaParty
{
    typedef std::function<void ()> Action;
    typedef std::function<bool ()> Condition;
    typedef std::function<bool (float)> TimedCondition;

    class TaskQueue
    {
    public:
        bool IsComplete() const;
        
        void Update(float deltaSeconds);
        
        TaskQueue& Perform(const Action &action);
        TaskQueue& Wait(float seconds);
        TaskQueue& WaitFor(const Condition &condition);
        TaskQueue& WaitUntil(const TimedCondition &timedCondition);
        
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
            float currentTime;
        };
        
        std::queue<Task> tasks;
    };
}

#endif//NINJAPARTY_TASKMANAGER_HPP

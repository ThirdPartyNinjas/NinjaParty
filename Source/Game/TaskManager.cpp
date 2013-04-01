#include <NinjaParty/TaskManager.hpp>

namespace NinjaParty
{
    bool TaskQueue::IsComplete() const
    {
        return tasks.empty();
    }
    
    void TaskQueue::Update(float deltaSeconds)
    {
        if(tasks.empty())
            return;
        
        Task &task = tasks.front();
        switch(task.taskType)
        {
            case TaskType::Action:
                task.action();
                tasks.pop();
                break;
                
            case TaskType::Condition:
                if(task.condition())
                    tasks.pop();
                break;
                
            case TaskType::TimedCondition:
                task.currentTime += deltaSeconds;
                if(task.timedCondition(task.currentTime))
                    tasks.pop();
                break;
        }
    }

    TaskQueue& TaskQueue::Perform(const Action &action)
    {
        tasks.push(Task(action));
        return *this;
    }
    
    TaskQueue& TaskQueue::Wait(float seconds)
    {
        return WaitUntil([=](float s){return s > seconds;});
    }
    
    TaskQueue& TaskQueue::WaitFor(const Condition &condition)
    {
        tasks.push(Task(condition));
        return *this;
    }
    
    TaskQueue& TaskQueue::WaitUntil(const TimedCondition &timedCondition)
    {
        tasks.push(Task(timedCondition));
        return *this;
    }
    
    TaskQueue::Task::Task(Action action)
        : taskType(TaskType::Action), action(action)
    {
    }
    
    TaskQueue::Task::Task(Condition condition)
        : taskType(TaskType::Condition), condition(condition)
    {
    }
    
    TaskQueue::Task::Task(TimedCondition timedCondition)
        : taskType(TaskType::TimedCondition), timedCondition(timedCondition), currentTime(0)
    {
    }
    
    TaskQueue::Task::Task(const Task &task)
        : taskType(task.taskType)
    {
        switch(task.taskType)
        {
            case TaskType::Action:
                action = task.action;
                break;
            case TaskType::Condition:
                condition = task.condition;
                break;
            case TaskType::TimedCondition:
                timedCondition = task.timedCondition;
                currentTime = task.currentTime;
                break;
        }
    }
    
    TaskQueue::Task::~Task()
    {
    }    
}

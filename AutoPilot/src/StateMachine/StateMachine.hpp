#ifndef AUTOPILOT_STATEMACHINE_STATEMACHINE_HPP_
#define AUTOPILOT_STATEMACHINE_STATEMACHINE_HPP_

class Event {
public:
    Event(uint64_t eventId, void* data):
        eventId(eventId), data(data)
    {}
    uint64_t getEventId()
    {
        return evenId;
    }
    void *getEventData()
    {
        return data;
    }

    template<typename T>
    T getEventData()
    {
        return  *(reinterpret_cast<T*>(data));
    }

private:
    uint64_t eventId;
    void* data;
}

class IState {
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void onEvent(IEvent& event) = 0;
}

class IFiniteStateMachine {
    virtual void changeState(IState& target) = 0;
}

#endif
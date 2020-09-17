#include <map>
#include <queue>

namespace shl {
namespace algorithm {

class Machine;

class EventBase {
public:
  virtual ~EventBase() {}
  EventBase(EventBase const&) = delete;
  virtual void operator=(EventBase const&) = delete;
protected:
  EventBase() {}
};

template <typename T>
class Event: public EventBase {
public:
  static T* id() {
    static T e;
    return &e;
  }
};

class StateBase {
public:
  virtual void enter(Machine *fsm){};
  virtual void exit(Machine *fsm){};
  virtual void update(Machine *fsm) = 0;
  virtual ~StateBase() {}

  StateBase(StateBase const&) = delete;
  virtual void operator=(StateBase const&) = delete;
protected:
  StateBase() {}
};

template <typename T>
class State: public StateBase {
public:
  static T* id() {
    static T s;
    return &s;
  }
};

class Machine {
  typedef std::queue<const EventBase*> EventQueue;
  typedef std::vector<const StateBase*> StateStack;
  typedef std::pair<const StateBase*, const EventBase*> EventPair;
  typedef std::map<const EventPair, StateBase*> TransitionMap;

  StateBase *currentState{0};
  StateStack stack;
  EventQueue eventQueue;
protected:
  TransitionMap transitions;

  virtual void setState(StateBase *state) {
    if (state == currentState) return;
    if (currentState) currentState->exit(this);
    currentState = state;
    if (currentState) currentState->enter(this);
  }

public:
  virtual ~Machine() {}

  virtual void signal(EventBase *event) {
    eventQueue.push(event);
  }

  virtual void defineTransition(
      const StateBase *from,
      const EventBase *event,
      StateBase *to) {
    transitions[std::make_pair(from, event)] = to;
  }

  virtual void update() {
    if (!eventQueue.empty()) {
      try {
        setState(transitions.at(std::make_pair(currentState, eventQueue.front())));
      } catch (const std::out_of_range &e) {
        try {
          setState(transitions.at(std::make_pair(currentState, nullptr)));
        } catch (const std::out_of_range &e) {
          //setState(transitions.at(std::make_pair(nullptr, nullptr)));
        }
      }
      eventQueue.pop();
    }
    if (currentState) currentState->update(this);
  }
};
}}

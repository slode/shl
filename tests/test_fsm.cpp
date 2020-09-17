#include "test/test.h"
#include "algorithm/fsm.h"

using namespace shl::test;
using namespace shl::algorithm;

#define TRACE \
    std::cout << __PRETTY_FUNCTION__ << std::endl;

class TurnOn: public Event<TurnOn> {};
class TurnOff: public Event<TurnOff> {};

class Off: public State<Off> {
public:
  void update(Machine* m) {
    TRACE
    m->signal(TurnOn::id());
  }
};

class On: public State <On>{
  int counter{0};
public:
  void enter(Machine *m) {
    TRACE
  }
  void update(Machine* m) {
    TRACE
    if (counter++ > 5)
      m->signal(TurnOn::id());
    if (counter++ > 2)
      m->signal(TurnOff::id());
  }
  void exit(Machine *m) {
    TRACE
  }
};

class GlobalDefault: public State <GlobalDefault>{
public:
  void update(Machine* m) {
    TRACE
    m->signal(TurnOff::id());
  }
};

class OnDefault: public State <OnDefault>{
public:
  void update(Machine* m) {
    TRACE
    m->signal(TurnOff::id());
  }
};

class JSM: public Machine {
  public:
  JSM() {
   defineTransition(0, 0, GlobalDefault::id());
   defineTransition(GlobalDefault::id(), 0, Off::id());
   defineTransition(On::id(), TurnOff::id(), Off::id());
   defineTransition(On::id(), 0, OnDefault::id());
   defineTransition(Off::id(), TurnOn::id(), On::id());
   signal(0);
  }
};

SHL_TEST_SUITE(FSM)

SHL_TEST_CASE(Initialization) {
  JSM mach;
  for (size_t i = 0; i<10; i++)
    mach.update();
}

SHL_TEST_SUITE_END

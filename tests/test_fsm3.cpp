#include "test/test.h"
#include "algorithm/fsm.h"

using namespace shl::test;
using namespace shl::algorithm;

#define TRACE \
    std::cout << __PRETTY_FUNCTION__ << std::endl;

struct Dwarf: public Machine {
  int drinks=0;
  int gold=6;
  int health = 5;
};

class IsDrunk: public Event<IsDrunk> {};
class IsRich: public Event<IsRich> {};
class IsPoor: public Event<IsPoor> {};
class IsHurt: public Event<IsHurt> {};

class Drinking: public State<Drinking> {
  void update(Machine *m) {
    TRACE
    Dwarf *d = reinterpret_cast<Dwarf*>(m);
    if (d->drinks++ > 5)
      d->signal(IsDrunk::id());
    if (d->gold-- <= 0)
      d->signal(IsPoor::id());
    if (d->health <= 5)
      d->health++;
  }
};

class Fighting: public State <Fighting>{
  void enter(Machine *m) {
    TRACE
  }
  void update(Machine* m) {
    TRACE
    Dwarf *d = reinterpret_cast<Dwarf*>(m);
    if (d->drinks > 0)
      d->drinks--;
    if (d->health-- <= 0)
      m->signal(IsHurt::id());
  }
  void exit(Machine *m) {
    TRACE
  }
};

class Mining: public State <Mining>{
public:
  void update(Machine* m) {
    TRACE
    Dwarf *d = reinterpret_cast<Dwarf*>(m);
    if (d->drinks > 0)
      d->drinks--;
    if (d->gold++ >= 5)
      d->signal(IsRich::id());
    if (d->health <= 5)
      d->health++;
  }
};

SHL_TEST_SUITE(FSM3)

SHL_TEST_CASE(Initialization) {
  Dwarf dwarf;
  dwarf.defineTransition(0, 0, Drinking::id());
  dwarf.defineTransition(Drinking::id(), IsDrunk::id(), Fighting::id());
  dwarf.defineTransition(Fighting::id(), IsHurt::id(),  Drinking::id());
  dwarf.defineTransition(Drinking::id(), IsPoor::id(),  Mining::id());
  dwarf.defineTransition(Mining::id(),   IsRich::id(),  Drinking::id());
  dwarf.signal(0);

  for (size_t i = 0; i<20; i++)
    dwarf.update();
}

SHL_TEST_SUITE_END

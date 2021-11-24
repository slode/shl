#include "test/test.h"
#include "algorithm/fsm.h"

using namespace shl::test;
using namespace shl::algorithm;

#define TRACE \
    std::cout << __PRETTY_FUNCTION__ << std::endl;

/** DAYLIGHT DEF **/
class Daylight: public Machine {
  uint hour_;
public:
  void incrementHour() {
    hour_++;
    hour_ %= 24;
  }
  uint getHour() const {
    return hour_;
  }
  bool isDay() const {
    return hour_ > 7 && hour_ < 19;
  }
};

class SunriseEvent: public Event<SunriseEvent> {};
class SunsetEvent: public Event<SunsetEvent> {};

class DayState: public State<DayState> {
public:
  void enter(Machine *m) {
    TRACE
    Daylight *day = static_cast<Daylight*>(m);
    std::cout << day->getHour() << std::endl;
  }
  void update(Machine* m) {
    Daylight *day = static_cast<Daylight*>(m);
    day->incrementHour();
    if (!day->isDay()) day->signal(SunsetEvent::id());
  }
  void exit(Machine *m) {
    TRACE
    Daylight *day = static_cast<Daylight*>(m);
    std::cout << day->getHour() << std::endl;
  }
};

class NightState: public State <NightState>{
  int counter{0};
public:
  void enter(Machine *m) {
    TRACE
    Daylight *day = static_cast<Daylight*>(m);
    std::cout << day->getHour() << std::endl;
  }
  void update(Machine* m) {
    Daylight *day = static_cast<Daylight*>(m);
    day->incrementHour();
    if (day->isDay()) day->signal(SunriseEvent::id());
  }
  void exit(Machine *m) {
    TRACE
    Daylight *day = static_cast<Daylight*>(m);
    std::cout << day->getHour() << std::endl;
  }
};

/** LAMP DEF **/
class LampOnEvent: public Event<LampOnEvent> {};
class LampOffEvent: public Event<LampOffEvent> {};

class Lamp: public Machine {
  int hours_on = 0;
  Daylight &day_;
public:
  Lamp(Daylight &day): day_(day)  {}
  const Daylight& getDaylight() { return day_; }
};

class LampOffState: public State<LampOffState> {
public:
  void enter(Machine *m) {
    TRACE
  }
  void update(Machine* m) {
    Lamp *lamp = static_cast<Lamp*>(m);
    if (lamp->getDaylight().isDay()) {
      lamp->signal(LampOnEvent::id());
    }
  }
  void exit(Machine *m) {
    TRACE
  }
};

class LampOnState: public State <LampOnState>{
  int counter{0};
public:
  void enter(Machine *m) {
    TRACE
  }
  void update(Machine* m) {
    Lamp *lamp = static_cast<Lamp*>(m);
    if (!lamp->getDaylight().isDay()) {
      lamp->signal(LampOffEvent::id());
    }
  }
  void exit(Machine *m) {
    TRACE
  }
};

SHL_TEST_SUITE(FSM)

SHL_TEST_CASE(Initialization) {
  Daylight day;
  day.defineTransition(0,                0,                  NightState::id());
  day.defineTransition(NightState::id(), SunriseEvent::id(), DayState::id());
  day.defineTransition(DayState::id(),   SunsetEvent::id(),  NightState::id());
  day.signal(0);

  Lamp lamp(day);
  lamp.defineTransition(0,                  0,                  LampOffState::id());
  lamp.defineTransition(LampOffState::id(), LampOnEvent::id(),  LampOnState::id());
  lamp.defineTransition(LampOnState::id(),  LampOffEvent::id(), LampOffState::id());
  lamp.signal(0);

  for (int i = 0; i<48; i++) {
    day.update();
    lamp.update();
  }
}

SHL_TEST_SUITE_END

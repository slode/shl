#include "test/test.h"
#include "algorithm/fsm.h"

using namespace shl::test;
using namespace shl::algorithm;

#define TRACE \
    std::cout << __PRETTY_FUNCTION__ << std::endl;

class JSM;
class MachineSelected:    public Event<MachineSelected> {};
class MachineDeselected:  public Event<MachineDeselected> {};
class MachineHalted:      public Event<MachineHalted> {};
class AxisJogSelected:    public Event<AxisJogSelected> {};
class RailJogSelected:    public Event<RailJogSelected> {};
class EEJogSelected:      public Event<EEJogSelected> {};
class JogModeChanged:     public Event<JogModeChanged> {};
class JoysticksReleased:  public Event<JoysticksReleased> {};
class JoysticksMoved:     public Event<JoysticksMoved> {};

enum class Mode {
  EE,
  Rail,
  Axis
};

class JSM: public Machine {
public:
  void selectMachine(bool selected) {
    TRACE;
    if (selected)
      signal(MachineSelected::id());
    else
      signal(MachineDeselected::id());
  }

  void moveJoysticks(int amplitude) {
    TRACE;
    if (amplitude > 0)
      signal(JoysticksMoved::id());
    else
      signal(JoysticksReleased::id());
  }

  void setMode(Mode m) {
    TRACE;
    mode = m;
    switch (m)
    {
      case Mode::Axis : signal(AxisJogSelected::id()); break;
      case Mode::Rail : signal(RailJogSelected::id()); break;
      case Mode::EE :   signal(EEJogSelected::id()); break;
    }
  }

  int machineVelocity{0};
  Mode mode;
};

class Off: public State<Off> {
public:
  void update(Machine* m) {
    TRACE;
  }
};

class Idle: public State<Idle> {
public:
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    jm->setMode(jm->mode);
  }
};

class Halting: public State<Halting> {
public:
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity <= 0) {
      jm->signal(MachineHalted::id());
    }
    std::cout << "Halting vel: " << jm->machineVelocity-- << std::endl;
  }
};

class JoggingEE: public State <JoggingEE>{
public:
  void enter(Machine *m) {
    TRACE;
  }
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity < 5)
      jm->machineVelocity++;
    std::cout << "EE vel: " << jm->machineVelocity << std::endl;
  }
};

class JoggingRailHalting: public State <JoggingRailHalting>{
public:
  void enter(Machine *m) {
    TRACE;
  }
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity > 0)
      jm->machineVelocity--;
    else if (jm->mode != Mode::Rail) {
      jm->setMode(jm->mode);
    }
    std::cout << "Rail vel: " << jm->machineVelocity << std::endl;
  }
};
class JoggingRail: public State <JoggingRail>{
public:
  void enter(Machine *m) {
    TRACE;
  }
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity < 5)
      jm->machineVelocity++;
    std::cout << "Rail vel: " << jm->machineVelocity << std::endl;
  }
};

class JoggingAxisHalting: public State <JoggingAxisHalting>{
public:
  void enter(Machine *m) {
    TRACE;
  }
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity > 0)
      jm->machineVelocity--;
    else if (jm->mode != Mode::Axis) {
      jm->setMode(jm->mode);
    }
    std::cout << "Axis vel: " << jm->machineVelocity << std::endl;
  }
};

class JoggingAxis: public State <JoggingAxis>{
public:
  void enter(Machine *m) {
    TRACE;
  }
  void update(Machine* m) {
    TRACE;
    JSM* jm = dynamic_cast<JSM*>(m);
    if (jm->machineVelocity < 5)
      jm->machineVelocity++;
    std::cout << "Axis vel: " << jm->machineVelocity << std::endl;
  }
};

SHL_TEST_SUITE(FSM2)

SHL_TEST_CASE(Initialization) {
  JSM mach;
  mach.defineTransition(0,              0,                       Off::id());
  mach.defineTransition(Off::id(),      MachineSelected::id(),   Idle::id());

  mach.defineTransition(Idle::id(),     0,                       Idle::id());
  mach.defineTransition(Idle::id(),     MachineDeselected::id(), Off::id());
  mach.defineTransition(Idle::id(),     AxisJogSelected::id(),   JoggingAxisHalting::id());
  mach.defineTransition(Idle::id(),     RailJogSelected::id(),   JoggingRailHalting::id());

//  mach.defineTransition(JoggingAxis::id(),        AxisJogSelected::id(),   JoggingAxis::id());
  mach.defineTransition(JoggingAxis::id(),        JoysticksMoved::id(),    JoggingAxis::id());
  mach.defineTransition(JoggingAxis::id(),        0,                       JoggingAxisHalting::id());
  mach.defineTransition(JoggingAxisHalting::id(), JoysticksMoved::id(),    JoggingAxis::id());
  mach.defineTransition(JoggingAxisHalting::id(), 0,                       Idle::id());
  mach.defineTransition(JoggingAxisHalting::id(), AxisJogSelected::id(),   JoggingAxisHalting::id());


//  mach.defineTransition(JoggingRail::id(),        RailJogSelected::id(),   JoggingRail::id());
  mach.defineTransition(JoggingRail::id(),        JoysticksMoved::id(),    JoggingRail::id());
  mach.defineTransition(JoggingRail::id(),        JoysticksReleased::id(), JoggingRailHalting::id());
  mach.defineTransition(JoggingRailHalting::id(), JoysticksMoved::id(),    JoggingRail::id());
  mach.defineTransition(JoggingRailHalting::id(), 0,                       Idle::id());

  mach.signal(0);
  mach.moveJoysticks(1);
  mach.update();
  mach.selectMachine(true);
  mach.update();
  mach.setMode(Mode::Axis);
  mach.update();
  mach.moveJoysticks(1);
  mach.update();
  for (int i=0;i<6;i++) mach.update();
  mach.moveJoysticks(0);
  for (int i=0;i<6;i++) mach.update();
  mach.setMode(Mode::Axis);
  mach.setMode(Mode::Rail);
  for (int i=0;i<6;i++) mach.update();
  mach.moveJoysticks(1);
  for (int i=0;i<6;i++) mach.update();
  mach.moveJoysticks(0);
  for (int i=0;i<7;i++) mach.update();
}

SHL_TEST_SUITE_END

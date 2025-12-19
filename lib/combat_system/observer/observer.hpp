#pragma once

#include <memory>
#include <string>

namespace lib::npc {
class NPC;
}

namespace lib::combat_system {

class Observer {
public:
  virtual void OnKill(const npc::NPC &killer, const npc::NPC &victim) = 0;
  virtual ~Observer() = default;
};

std::shared_ptr<Observer> MakeConsoleObserver();
std::shared_ptr<lib::combat_system::Observer>
MakeFileObserver(const std::string &filename);
} // namespace lib::combat_system
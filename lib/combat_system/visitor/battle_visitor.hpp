#pragma once

#include <memory>
#include <vector>

namespace lib::npc {
class NPC;
class Princess;
class Dragon;
class Knight;
} // namespace lib::npc

namespace lib::combat_system {
class Observer;

class Visitor {
public:
  Visitor(std::shared_ptr<npc::NPC> attacker,
          const std::vector<std::shared_ptr<Observer>> &observers = {});

  bool Fight(npc::Princess &target);
  bool Fight(npc::Dragon &target);
  bool Fight(npc::Knight &target);

private:
  std::shared_ptr<npc::NPC> attacker_;
  std::vector<std::shared_ptr<Observer>> observers_;

  void NotifyKill(const npc::NPC &killer, const npc::NPC &victim);
};
} // namespace lib::combat_system
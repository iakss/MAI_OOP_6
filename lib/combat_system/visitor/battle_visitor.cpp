#include "battle_visitor.hpp"
#include "dragon.hpp"
#include "knight.hpp"
#include "observer.hpp"
#include "princess.hpp"

namespace lib::combat_system {

Visitor::Visitor(std::shared_ptr<npc::NPC> attacker,
                 const std::vector<std::shared_ptr<Observer>> &observers)
    : attacker_(std::move(attacker)), observers_(observers) {}

void Visitor::NotifyKill(const npc::NPC &killer, const npc::NPC &victim) {
  for (auto &obs : observers_) {
    if (obs)
      obs->OnKill(killer, victim);
  }
}

bool Visitor::Fight(npc::Princess &target) {
  if (!attacker_)
    return false;
  if (!attacker_->IsAlive())
    return false;

  if (attacker_->GetType() == npc::NPCType::Dragon) {
    NotifyKill(*attacker_, target);
    return true;
  }
  return false;
}

bool Visitor::Fight(npc::Dragon &target) {
  if (!attacker_)
    return false;
  if (!attacker_->IsAlive())
    return false;

  if (attacker_->GetType() == npc::NPCType::Knight) {
    NotifyKill(*attacker_, target);
    return true;
  }
  return false;
}

bool Visitor::Fight(npc::Knight &) { return false; }

} // namespace lib::combat_system

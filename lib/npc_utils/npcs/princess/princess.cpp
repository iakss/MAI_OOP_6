#include "princess.hpp"

namespace lib::npc {

Princess::Princess(const std::string &name, Point pos) : NPC(name, pos) {}

NPCType Princess::GetType() const { return NPCType::Princess; }

std::string_view Princess::GetTypename() const { return "Princess"; }

bool Princess::AcceptFight(combat_system::Visitor &visitor) {
  return visitor.Fight(*this);
}
} // namespace lib::npc
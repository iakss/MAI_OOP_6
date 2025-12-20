#include "dragon.hpp"
#include "battle_visitor.hpp"

namespace lib::npc {

Dragon::Dragon(const std::string &name, Point pos) : NPC(name, pos) {}

NPCType Dragon::GetType() const { return NPCType::Dragon; }

std::string_view Dragon::GetTypename() const { return "Dragon"; }

bool Dragon::AcceptFight(combat_system::Visitor &visitor) {
  return visitor.Fight(*this);
}
} // namespace lib::npc
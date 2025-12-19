#include "knight.hpp"

namespace lib::npc {

Knight::Knight(const std::string &name, Point pos) : NPC(name, pos) {}

NPCType Knight::GetType() const { return NPCType::Knight; }

std::string_view Knight::GetTypename() const { return "Knight"; }
bool Knight::AcceptFight(combat_system::Visitor &visitor) {
  return visitor.Fight(*this);
}
} // namespace lib::npc
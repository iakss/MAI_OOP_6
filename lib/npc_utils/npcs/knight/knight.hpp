#pragma once

#include "npc.hpp"

namespace lib::npc {

class Knight : public NPC {
public:
  Knight(const std::string &name, Point pos);

  NPCType GetType() const final;
  std::string_view GetTypename() const final;

  bool AcceptFight(combat_system::Visitor &visitor) final;
};
} // namespace lib::npc
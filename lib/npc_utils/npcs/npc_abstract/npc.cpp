#include "npc.hpp"

#include <cmath>

namespace lib::npc {
bool operator==(const Point &first, const Point &second) noexcept {
  return first.x == second.x && first.y == second.y;
}

double Distance(const Point &first, const Point &second) noexcept {
  return std::sqrt(std::pow(first.x - second.x, 2) +
                   std::pow(first.y - second.y, 2));
}

NPC::NPC(const std::string &name, Point pos) : name_(name), pos_(pos) {}

std::string_view NPC::GetName() const noexcept {
  return std::string_view(name_);
}

Point NPC::GetPos() const noexcept { return pos_; }

bool NPC::IsAlive() const noexcept { return alive_; }

void NPC::Kill() noexcept { alive_ = false; }
} // namespace lib::npc
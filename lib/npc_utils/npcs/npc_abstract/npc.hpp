#pragma once

#include "battle_visitor.hpp"

#include <string>
#include <string_view>

namespace lib::npc {

struct Point {
  double x;
  double y;
  friend bool operator==(const Point &first, const Point &second) noexcept;
};

double Distance(const Point &first, const Point &second) noexcept;

enum class NPCType { Princess, Dragon, Knight };

class NPC {
public:
  NPC(const std::string &name, Point pos);

  std::string_view GetName() const noexcept;
  Point GetPos() const noexcept;
  bool IsAlive() const noexcept;
  void Kill() noexcept;

  virtual NPCType GetType() const = 0;
  virtual std::string_view GetTypename() const = 0;

  virtual bool AcceptFight(combat_system::Visitor &visitor) = 0;

  virtual ~NPC() = default;

protected:
  bool alive_ = true;

  std::string name_;
  Point pos_;
};
} // namespace lib::npc
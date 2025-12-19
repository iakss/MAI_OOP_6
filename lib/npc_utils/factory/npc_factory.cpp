#include "npc_factory.hpp"
#include "dragon.hpp"
#include "exceptions.hpp"
#include "knight.hpp"
#include "princess.hpp"

#include <sstream>
#include <stdexcept>

namespace {
static bool IsValidCoordinates(double x, double y) {
  return x >= 0.0 && x <= 500.0 && y >= 0.0 && y <= 500.0;
}

bool ReadTypeFromString(std::string_view str, lib::npc::NPCType &type) {
  if (str == "Princess") {
    type = lib::npc::NPCType::Princess;
    return true;
  }
  if (str == "Dragon") {
    type = lib::npc::NPCType::Dragon;
    return true;
  }
  if (str == "Knight") {
    type = lib::npc::NPCType::Knight;
    return true;
  }
  return false;
}
} // namespace

namespace lib::npc {

std::shared_ptr<NPC> NPCFactory::Create(NPCType type, const std::string &name,
                                        Point pos) {
  if (!IsValidCoordinates(pos.x, pos.y)) {
    exception::InvalidCoordinates("Coordinates must be in [0, 500]!");
  }

  switch (type) {
  case NPCType::Princess:
    return std::make_shared<Princess>(name, pos);
  case NPCType::Dragon:
    return std::make_shared<Dragon>(name, pos);
  case NPCType::Knight:
    return std::make_shared<Knight>(name, pos);
  }
  return nullptr;
}

std::shared_ptr<NPC> NPCFactory::CreateFromString(std::string_view type_str,
                                                  const std::string &name,
                                                  Point pos) {
  NPCType npc_type;
  if (!ReadTypeFromString(type_str, npc_type)) {
    return nullptr;
  }
  return NPCFactory::Create(npc_type, name, pos);
}

std::shared_ptr<NPC>
NPCFactory::CreateNPCFromFileLine(const std::string &line) {
  std::istringstream iss(line);
  std::string type;
  std::string name;
  double x, y;
  if (!(iss >> type >> name >> x >> y)) {
    throw std::runtime_error("Bad line format: " + line);
  }
  return CreateFromString(type, name, Point{x, y});
}
} // namespace lib::npc

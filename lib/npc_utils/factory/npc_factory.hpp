#pragma once

#include "npc.hpp"

#include <memory>

namespace lib::npc {

class NPCFactory {
public:
  NPCFactory() = default;

  std::shared_ptr<NPC> Create(NPCType type, const std::string &name, Point pos);

  std::shared_ptr<NPC> CreateFromString(std::string_view type_str,
                                        const std::string &name, Point pos);

  std::shared_ptr<NPC> CreateNPCFromFileLine(const std::string &line);
};
} // namespace lib::npc
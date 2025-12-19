#pragma once

#include "npc.hpp"
#include "observer.hpp"

namespace lib::simulation {

class World {
public:
  World() = default;

  void AddNPC(std::shared_ptr<lib::npc::NPC> npc);

  void SaveToFile(const std::string &filename) const;
  void LoadFromFile(const std::string &filename);

  void PrintAll() const;

  size_t RunBattle(double range);

  void AddObserver(std::shared_ptr<lib::combat_system::Observer> observer);

private:
  std::vector<std::shared_ptr<lib::npc::NPC>> npcs_;
  std::vector<std::shared_ptr<lib::combat_system::Observer>> observers_;

  bool NameExists(const std::string &name) const;
  void RemoveDead();
};

} // namespace lib::simulation

#include "world.hpp"
#include "battle_visitor.hpp"
#include "npc_factory.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace lib::simulation {

void World::AddObserver(
    std::shared_ptr<lib::combat_system::Observer> observer) {
  observers_.push_back(std::move(observer));
}

bool World::NameExists(const std::string &name) const {
  for (const auto &p : npcs_) {
    if (p && p->GetName() == name)
      return true;
  }
  return false;
}

void World::AddNPC(std::shared_ptr<lib::npc::NPC> npc) {
  if (!npc)
    throw std::runtime_error("Null NPC");
  if (NameExists(std::string(npc->GetName())))
    throw std::runtime_error("NPC with this name already exists: " +
                             std::string(npc->GetName()));
  npcs_.push_back(std::move(npc));
}

void World::SaveToFile(const std::string &filename) const {
  std::ofstream ofs(filename);
  if (!ofs)
    throw std::runtime_error("Can't open file for writing: " + filename);

  for (const auto &p : npcs_) {
    if (!p)
      continue;
    auto type = p->GetTypename();
    auto name = p->GetName();
    auto pos = p->GetPos();
    ofs << std::string(type) << ' ' << std::string(name) << ' ' << pos.x << ' '
        << pos.y << '\n';
  }
}

void World::LoadFromFile(const std::string &filename) {
  std::ifstream ifs(filename);
  if (!ifs)
    throw std::runtime_error("Can't open file: " + filename);

  lib::npc::NPCFactory factory;
  std::string line;

  while (std::getline(ifs, line)) {
    if (line.empty())
      continue;

    auto npc = factory.CreateNPCFromFileLine(line);
    if (NameExists(std::string(npc->GetName()))) {
      throw std::runtime_error("Duplicate NPC name: " +
                               std::string(npc->GetName()));
    }
    npcs_.push_back(std::move(npc));
  }
}

void World::PrintAll() const {
  std::cout << "NPCs (" << npcs_.size() << "):\n";
  for (const auto &p : npcs_) {
    if (!p)
      continue;
    auto type = p->GetTypename();
    auto name = p->GetName();
    auto pos = p->GetPos();
    std::cout << "- " << std::string(type) << " '" << std::string(name)
              << "' at (" << pos.x << ", " << pos.y << ")"
              << (p->IsAlive() ? "" : " [dead]") << "\n";
  }
}

void World::RemoveDead() {
  npcs_.erase(std::remove_if(npcs_.begin(), npcs_.end(),
                             [](const std::shared_ptr<lib::npc::NPC> &p) {
                               return !p || !p->IsAlive();
                             }),
              npcs_.end());
}

size_t World::RunBattle(double range) {
  if (range < 0.0)
    return 0;
  size_t removed = 0;

  for (size_t i = 0; i < npcs_.size(); ++i) {
    for (size_t j = i + 1; j < npcs_.size(); ++j) {
      auto &a = npcs_[i];
      auto &b = npcs_[j];
      if (!a || !b)
        continue;
      if (!a->IsAlive() || !b->IsAlive())
        continue;
      double d = lib::npc::Distance(a->GetPos(), b->GetPos());
      if (d > range)
        continue;

      lib::combat_system::Visitor vis_a(a, observers_);
      if (b->AcceptFight(vis_a)) {
        b->Kill();
        ++removed;
        continue;
      }
      lib::combat_system::Visitor vis_b(b, observers_);
      if (a->AcceptFight(vis_b)) {
        a->Kill();
        ++removed;
      }
    }
  }

  RemoveDead();
  return removed;
}

} // namespace lib::simulation

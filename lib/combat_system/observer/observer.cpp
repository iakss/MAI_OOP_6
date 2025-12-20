#include "observer.hpp"
#include "npc.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace lib::combat_system {

class ConsoleObserver : public Observer {
public:
  void OnKill(const npc::NPC &killer, const npc::NPC &victim) final {
    std::cout << killer.GetTypename() << " '" << killer.GetName() << "' killed "
              << victim.GetTypename() << " '" << victim.GetName() << "'\n";
  }
};

class FileObserver : public Observer {
public:
  explicit FileObserver(const std::string &filename = "log.txt")
      : filename_(filename) {}

  void OnKill(const npc::NPC &killer, const npc::NPC &victim) final {
    std::ofstream file(filename_, std::ios::app);
    if (!file)
      throw std::runtime_error("Bad file opening!");
    file << killer.GetTypename() << " '" << killer.GetName() << "' killed "
         << victim.GetTypename() << " '" << victim.GetName() << "'\n";
  }

private:
  std::string filename_;
};

std::shared_ptr<lib::combat_system::Observer> MakeConsoleObserver() {
  return std::make_shared<lib::combat_system::ConsoleObserver>();
}

std::shared_ptr<lib::combat_system::Observer>
MakeFileObserver(const std::string &filename) {
  return std::make_shared<lib::combat_system::FileObserver>(filename);
}
} // namespace lib::combat_system
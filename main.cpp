// main.cpp
#include "npc_factory.hpp"
#include "observer.hpp"
#include "world.hpp"
#include <iostream>

int main() {
  try {
    lib::simulation::World world;

    world.AddObserver(lib::combat_system::MakeConsoleObserver());
    world.AddObserver(lib::combat_system::MakeFileObserver("log.txt"));

    lib::npc::NPCFactory factory;

    try {
      world.AddNPC(factory.CreateFromString("Princess", "Anna", {10.0, 10.0}));
      world.AddNPC(factory.CreateFromString("Dragon", "Draco", {12.0, 12.0}));
      world.AddNPC(
          factory.CreateFromString("Knight", "Lancelot", {400.0, 400.0}));
      world.AddNPC(factory.CreateFromString("Dragon", "Smaug", {402.0, 402.0}));
      world.AddNPC(factory.CreateFromString("Princess", "Belle", {50.0, 50.0}));
    } catch (const std::exception &e) {
      std::cerr << "Error while adding NPCs: " << e.what() << "\n";
    }

    std::cout << "Initial world:\n";
    world.PrintAll();

    const std::string save_file = "save.txt";
    try {
      world.SaveToFile(save_file);
      std::cout << "World saved to '" << save_file << "'\n";
    } catch (const std::exception &e) {
      std::cerr << "Save failed: " << e.what() << "\n";
    }

    lib::simulation::World loaded;
    loaded.AddObserver(lib::combat_system::MakeConsoleObserver());
    loaded.AddObserver(lib::combat_system::MakeFileObserver("log.txt"));

    try {
      loaded.LoadFromFile(save_file);
      std::cout << "\nLoaded world from '" << save_file << "':\n";
      loaded.PrintAll();
    } catch (const std::exception &e) {
      std::cerr << "Load failed: " << e.what() << "\n";
    }

    const double battle_range = 5.0;
    std::cout << "\nRunning battle with range = " << battle_range << "...\n";
    try {
      size_t removed = loaded.RunBattle(battle_range);
      std::cout << "Number of removed (killed) NPCs: " << removed << "\n";
    } catch (const std::exception &e) {
      std::cerr << "Battle failed: " << e.what() << "\n";
    }

    std::cout << "\nWorld after battle:\n";
    loaded.PrintAll();

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}

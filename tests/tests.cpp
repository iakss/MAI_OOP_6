#include <gtest/gtest.h>

#include "exceptions.hpp"
#include "npc_factory.hpp"
#include "observer.hpp"
#include "world.hpp"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

/* ---------- Helpers ---------- */

static std::string ReadFileAll(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs)
    return {};
  return std::string((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
}

static void RemoveIfExists(const std::string &path) {
  std::remove(path.c_str());
}

/* ---------- Test Observer (for checking in-memory notifications) ---------- */

class TestObserver : public lib::combat_system::Observer {
public:
  void OnKill(const lib::npc::NPC &killer,
              const lib::npc::NPC &victim) override {
    events.emplace_back(std::string(killer.GetName()),
                        std::string(victim.GetName()));
  }

  std::vector<std::pair<std::string, std::string>> events;
};

/* ---------- Tests ---------- */

TEST(NPCFactory, CreatesAllTypes) {
  lib::npc::NPCFactory f;
  auto p = f.CreateFromString("Princess", "Anna", lib::npc::Point{1.0, 2.0});
  auto d = f.CreateFromString("Dragon", "Draco", lib::npc::Point{10.0, 10.0});
  auto k =
      f.CreateFromString("Knight", "Lancelot", lib::npc::Point{20.0, 20.0});

  ASSERT_NE(p, nullptr);
  ASSERT_NE(d, nullptr);
  ASSERT_NE(k, nullptr);

  EXPECT_EQ(p->GetType(), lib::npc::NPCType::Princess);
  EXPECT_EQ(d->GetType(), lib::npc::NPCType::Dragon);
  EXPECT_EQ(k->GetType(), lib::npc::NPCType::Knight);

  EXPECT_EQ(std::string(p->GetName()), "Anna");
  EXPECT_EQ(std::string(d->GetName()), "Draco");
  EXPECT_EQ(std::string(k->GetName()), "Lancelot");
}

TEST(NPCFactory, ThrowsOnInvalidCoordinates) {
  lib::npc::NPCFactory f;
  EXPECT_THROW(f.CreateFromString("Princess", "X", lib::npc::Point{600.0, 0.0}),
               lib::exception::InvalidCoordinates);
  EXPECT_THROW(f.CreateFromString("Dragon", "Y", lib::npc::Point{-1.0, 0.0}),
               lib::exception::InvalidCoordinates);
}

TEST(World, RejectsDuplicateNames) {
  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddNPC(f.CreateFromString("Princess", "Anna", lib::npc::Point{0.0, 0.0}));
  EXPECT_THROW(w.AddNPC(f.CreateFromString("Dragon", "Anna",
                                           lib::npc::Point{10.0, 10.0})),
               std::runtime_error);
}

TEST(World, SaveLoadRoundtrip) {
  const std::string fname = "test_save.txt";
  RemoveIfExists(fname);

  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddNPC(f.CreateFromString("Princess", "Anna", lib::npc::Point{1.0, 1.0}));
  w.AddNPC(f.CreateFromString("Dragon", "Draco", lib::npc::Point{5.0, 5.0}));

  ASSERT_NO_THROW(w.SaveToFile(fname));
  std::string content = ReadFileAll(fname);
  EXPECT_FALSE(content.empty());
  EXPECT_NE(content.find("Anna"), std::string::npos);
  EXPECT_NE(content.find("Draco"), std::string::npos);

  lib::simulation::World loaded;
  ASSERT_NO_THROW(loaded.LoadFromFile(fname));

  RemoveIfExists(fname);
}

TEST(CombatRules, DragonKillsPrincess) {
  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddNPC(f.CreateFromString("Princess", "Anna", lib::npc::Point{10.0, 10.0}));
  w.AddNPC(f.CreateFromString("Dragon", "Draco", lib::npc::Point{12.0, 12.0}));
  size_t removed = w.RunBattle(5.0);
  EXPECT_EQ(removed, 1u);
}

TEST(CombatRules, KnightKillsDragon) {
  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddNPC(
      f.CreateFromString("Dragon", "Smaug", lib::npc::Point{100.0, 100.0}));
  w.AddNPC(
      f.CreateFromString("Knight", "Arthur", lib::npc::Point{102.0, 102.0}));
  size_t removed = w.RunBattle(5.0);
  EXPECT_EQ(removed, 1u);
}

TEST(CombatRules, PrincessNeverAttacks) {
  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddNPC(f.CreateFromString("Princess", "Belle", lib::npc::Point{0.0, 0.0}));
  w.AddNPC(f.CreateFromString("Knight", "Arthur", lib::npc::Point{1.0, 1.0}));
  // Knight does not eat Princess per rules; Princess never attacks.
  size_t removed = w.RunBattle(5.0);
  EXPECT_EQ(removed, 0u);
}

TEST(ObserverTests, InMemoryObserverGetsNotified) {
  lib::simulation::World w;
  lib::npc::NPCFactory f;
  auto testObs = std::make_shared<TestObserver>();
  w.AddObserver(testObs);

  w.AddNPC(f.CreateFromString("Princess", "Anna", lib::npc::Point{10.0, 10.0}));
  w.AddNPC(f.CreateFromString("Dragon", "Draco", lib::npc::Point{12.0, 12.0}));

  size_t removed = w.RunBattle(5.0);
  EXPECT_EQ(removed, 1u);

  // One event must be recorded: Draco killed Anna
  ASSERT_EQ(testObs->events.size(), 1u);
  EXPECT_EQ(testObs->events[0].first, "Draco");
  EXPECT_EQ(testObs->events[0].second, "Anna");
}

TEST(ObserverTests, FileObserverWritesLog) {
  const std::string logfile = "test_log.txt";
  RemoveIfExists(logfile);

  lib::simulation::World w;
  lib::npc::NPCFactory f;
  w.AddObserver(lib::combat_system::MakeFileObserver(logfile));

  w.AddNPC(f.CreateFromString("Princess", "Anna", lib::npc::Point{10.0, 10.0}));
  w.AddNPC(f.CreateFromString("Dragon", "Draco", lib::npc::Point{12.0, 12.0}));

  size_t removed = w.RunBattle(5.0);
  EXPECT_EQ(removed, 1u);

  std::string log = ReadFileAll(logfile);
  EXPECT_FALSE(log.empty());
  EXPECT_NE(log.find("Draco"), std::string::npos);
  EXPECT_NE(log.find("Anna"), std::string::npos);

  RemoveIfExists(logfile);
}

/* ---------- main ---------- */

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

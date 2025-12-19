cmake_minimum_required(VERSION 3.16)

project(npc_editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_library(npc_core STATIC
    lib/npc_utils/factory/npc_factory.cpp
    lib/npc_utils/npcs/npc_abstract/npc.cpp
    lib/npc_utils/npcs/princess/princess.cpp
    lib/npc_utils/npcs/dragon/dragon.cpp
    lib/npc_utils/npcs/knight/knight.cpp
    lib/world/world.cpp
)

target_include_directories(npc_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib 

        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/factory

        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/npc_abstract
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/princess
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/dragon
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/knight
)

add_library(combat_system STATIC
    lib/combat_system/observer/observer.cpp
    lib/combat_system/visitor/battle_visitor.cpp
)

target_include_directories(combat_system
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/observer
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/visitor
)

target_link_libraries(combat_system npc_core)

add_library(sim STATIC lib/world/world.cpp)

target_include_directories(
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/world
)

target_link_libraries(sim combat_system npc_core)

add_executable(editor main.cpp)
target_link_libraries(editor PRIVATE npc_core combat_system sim)

add_executable(tests
    tests/tests.cpp
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(tests PRIVATE npc_core combat_system sim gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
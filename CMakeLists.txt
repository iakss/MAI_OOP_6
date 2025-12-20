cmake_minimum_required(VERSION 3.16)
project(npc_editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(npc STATIC
    lib/npc_utils/npcs/npc_abstract/npc.cpp

    lib/npc_utils/npcs/princess/princess.cpp
    lib/npc_utils/npcs/dragon/dragon.cpp
    lib/npc_utils/npcs/knight/knight.cpp

    lib/npc_utils/factory/npc_factory.cpp
)

target_include_directories(npc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/visitor
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/observer
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/factory
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
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/observer
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/visitor
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/npc_abstract
)

target_link_libraries(combat_system PUBLIC npc)

add_library(simulation STATIC
    lib/world/world.cpp
)

target_include_directories(simulation
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/world
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/factory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/npc_utils/npcs/npc_abstract
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/observer
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/combat_system/visitor
)

target_link_libraries(simulation
    PUBLIC
        npc
        combat_system
)

target_include_directories(npc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(combat_system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(editor main.cpp)
target_link_libraries(editor PRIVATE simulation)
target_include_directories(editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/tests.cpp
)

target_compile_features(tests PRIVATE cxx_std_20)

target_link_libraries(tests PRIVATE simulation gtest_main)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

include(GoogleTest)
gtest_discover_tests(tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
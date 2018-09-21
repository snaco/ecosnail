#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ecosnail/thing.hpp>

namespace th = ecosnail::thing;

struct C1 {
    int id;
};

struct C2 {
    int id;
};

TEST_CASE("Simple", "[simple]")
{
    th::EntityManager manager;

    auto e1 = manager.createEntity();
    auto e2 = manager.createEntity();
    auto e12 = manager.createEntity();

    manager.add<C1>(e1);
    manager.component<C1>(e1).id = 1;

    manager.add<C2>(e2);
    manager.component<C2>(e2).id = 2;

    manager.add<C1>(e12);
    manager.component<C1>(e12).id = 3;

    manager.add<C2>(e12);
    manager.component<C2>(e12).id = 4;

    for (const C1& component : manager.componentsOfType<C1>()) {
        REQUIRE(component.id % 2 == 1);
    }
    for (const C2& component : manager.componentsOfType<C2>()) {
        REQUIRE(component.id % 2 == 0);
    }

    for (const th::Entity& entity : manager.entities<C1>()) {
        REQUIRE(entity != e2);
    }
    for (const th::Entity& entity : manager.entities<C2>()) {
        REQUIRE(entity != e1);
    }
}

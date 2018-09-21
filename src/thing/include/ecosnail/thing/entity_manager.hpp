#pragma once

#include <ecosnail/thing/entity.hpp>
#include <ecosnail/thing/entity_pool.hpp>
#include <ecosnail/thing/range.hpp>

#include <any>
#include <map>
#include <typeindex>
#include <vector>

namespace ecosnail::thing {

class EntityManager {
public:
    template <class Component>
    const Component& component(Entity entity) const
    {
        return componentMap<Component>().at(entity);
    }

    template <class Component>
    Component& component(Entity entity)
    {
        return componentMap<Component>()[entity];
    }

    template <class Component>
    const auto componentsOfType()
    {
        return Range<Component, Get::Components>(componentMap<Component>());
    }

    template <class Component>
    const auto entities()
    {
        return Range<Component, Get::Entities>(componentMap<Component>());
    }

    template <class Component>
    void add(Entity entity)
    {
        // TODO: check that entity does not yet have this component
        auto anyComponentMap = _components.find(typeid(Component));
        if (anyComponentMap != _components.end()) {
            auto& componentMap =
                std::any_cast<ComponentMap<Component>&>(anyComponentMap->second);
            componentMap[entity] = Component();
        }

        ComponentMap<Component> componentMap;
        componentMap[entity] = Component();
        _components[typeid(Component)] = std::any(std::move(componentMap));
    }

    Entity createEntity()
    {
        return _entityPool.createEntity();
    }

    void killEntity(Entity entity)
    {
        _entityPool.killEntity(entity);
    }

private:
    template <class Component>
    auto& componentMap()
    {
        return std::any_cast<ComponentMap<Component>&>(
            _components[typeid(Component)]);
    }

    EntityPool _entityPool;
    std::map<std::type_index, std::any> _components;
};

} // namespace ecosnail::thing

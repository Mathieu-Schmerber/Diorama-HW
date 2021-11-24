#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <memory>
#include "Utils.h"
#include "Entity.h"

class System 
{
protected:
    std::vector<Component *> _dependencies;
    std::vector<Entity *> _entities;

public:
    explicit System() = default;

    template<typename T> void AddDependency();

    void AddEntity(Entity *entity)
    {
        this->_entities.emplace_back(entity);
    }

    std::vector<Component *>& GetDependencies()
    {
        return this->_dependencies;
    }

    std::vector<Entity *>& GetEntities()
    {
        return this->_entities;
    }

    virtual void LoadSystem() {}

    virtual void Update(float deltatime, float time) {}
};

template<typename T> void System::AddDependency()
{
   Component* component = new T();

    component->SetInfo(typeid(T).name());
    this->_dependencies.push_back(component);
}


#endif // !SYSTEM_H
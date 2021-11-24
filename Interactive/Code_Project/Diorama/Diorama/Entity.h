#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include "Component.h"

class Entity {

protected:
    std::vector<Component *> _components;

public:
    explicit Entity() = default;

    ~Entity()
    {
        for (auto& c : this->_components)
            delete c;
        this->_components.clear();
    }

    template<typename T, typename... TArgs> T* AddComponent(TArgs&&... args);

    template<typename T> [[nodiscard]] T* GetComponent() const;

    template<typename T> [[nodiscard]] std::vector<T*> GetComponents() const;

    bool HasComponents(std::vector<Component*>& components)
    {
        size_t match = 0;

        for (auto& comp : components)
        {
            for (auto& my : this->_components)
            {
                if (comp->GetInfo() == my->GetInfo())
                {
                    match++;
                    break;
                }
            }
        }
        return (match >= components.size());
    }
};

template<typename T, typename... TArgs>
T* Entity::AddComponent(TArgs &&... args)
{
    auto component = new T(std::forward<TArgs>(args)...);

    component->SetInfo(typeid(T).name());
    this->_components.push_back(component);
    return static_cast<T*>(this->_components.back());
}

template<typename T>
T* Entity::GetComponent() const
{
    for (auto& component : this->_components)
    {
        if (typeid(T).name() == component->GetInfo())
            return static_cast<T*>(component);
    }
    return nullptr;
}

template<typename T>
std::vector<T*> Entity::GetComponents() const
{
    std::vector<T*> res = {};

    for (auto& component : this->_components)
    {
        if (typeid(T).name() == component->GetInfo())
            res.push_back(static_cast<T*>(component));
    }
    return res;
}
#endif // !ENTITY_H
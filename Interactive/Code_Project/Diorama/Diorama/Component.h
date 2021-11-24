#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class Component
{
protected:
    std::string _info;

public:
    explicit Component() = default;
    ~Component() = default;

    void SetInfo(const std::string& info)
    {
        this->_info = info;
    }

    std::string GetInfo() const
    {
        return this->_info;
    }
};

#endif // !COMPONENT_H
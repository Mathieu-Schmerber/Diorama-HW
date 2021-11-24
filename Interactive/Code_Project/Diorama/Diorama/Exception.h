#pragma once

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>

class Exception : public std::exception
{
public:
    explicit Exception(const std::string &source = "Error", const std::string &message = "An error occurred.") :
        _source(source), _message(message)
    {
    };

    [[nodiscard]] std::string const& getComponent() const
    {
        return this->_source;
    };

    [[nodiscard]] const char* what() const noexcept override
    {
        return this->_message.c_str();
    };

    [[nodiscard]] std::string getFmtMessage() const
    {
        return "\033[31;1m" + _source + "\033[0;0m: " + _message;
    }

private:
    std::string _source;
    std::string _message;
};

#endif // !EXCEPTION_H
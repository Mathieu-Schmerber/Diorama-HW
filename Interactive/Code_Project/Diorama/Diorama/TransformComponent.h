#pragma once

#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>
#include "Component.h"

class TransformComponent : public Component 
{
private:
    glm::vec3 _up;
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
    glm::vec3 _forward;

public:
    explicit TransformComponent(const glm::vec3& pos = { 0.0, 0.0, 0.0 },
        const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
        const glm::vec3& scale = { 1, 1, 1 },
        const glm::vec3& up = { 0, 1, 0 })
        : _position(pos), _scale(scale), _up(up), _forward({ 0, 0, 1 }), Component()
    {
        SetRotation(rotation);
    }

    [[nodiscard]] glm::vec3 position() const { return this->_position; }
    void SetPosition(const glm::vec3& pos) { this->_position = pos; }
    void MoveBy(const glm::vec3& increment)
    {
        this->_position += increment;
    }

    [[nodiscard]] glm::vec3 rotation() const { return this->_rotation; }
    void SetRotation(const glm::vec3& rotation)
    {
        this->_rotation = glm::radians(rotation);
        this->_forward = glm::rotateX(this->_forward, glm::radians(rotation.x));
        this->_forward = glm::rotateY(this->_forward, glm::radians(rotation.y));
        this->_forward = glm::rotateZ(this->_forward, glm::radians(rotation.z));
    }

    [[nodiscard]] glm::vec3 forward() const { return this->_forward; }
    void SetForward(const glm::vec3 value)
    {
        this->_forward = value;

        glm::mat4 rotMat = glm::lookAt(_position, _position + _forward, _up);
        // to euler
        float sy = sqrt(rotMat[0][0] * rotMat[0][0] + rotMat[1][0] * rotMat[1][0]);
        bool singular = sy < 1e-6;
        float x, y, z;
        if (!singular)
        {
            x = atan2(rotMat[2][1], rotMat[2][2]);
            y = atan2(-rotMat[2][0], sy);
            z = atan2(rotMat[1][0], rotMat[0][0]);
        }
        else
        {
            x = atan2(-rotMat[1][2], rotMat[1][1]);
            y = atan2(-rotMat[2][0], sy);
            z = 0;
        }
        this->_rotation = glm::vec3(x, y, z);
    }

    glm::mat4 viewMatrix()
    {
        return glm::lookAt(_position, _position + _rotation, _up);
    }

    [[nodiscard]] glm::vec3 up() const { return this->_up; }
    [[nodiscard]] glm::mat4 matrix() const 
    { 
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), this->_position);
        modelMatrix = glm::rotate(modelMatrix, this->_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, this->_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, this->_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, this->_scale);
        return modelMatrix;
    }
};

#endif // !TRANSFORM_COMPONENT_H
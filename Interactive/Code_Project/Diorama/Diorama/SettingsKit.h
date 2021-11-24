#pragma once

#ifndef SETTINGS_KIT_H
#define SETTINGS_KIT_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>

#include "Model.h"
#include "Button.h"

class SettingsKit
{
private:
	std::vector<Entity*> _settingActors;

public:
	SettingsKit(const glm::vec3& position, float ratio, void (*onclick)(float), const std::string &iconFile)
	{
		Model* title = new Model(iconFile, position + glm::vec3(0, 3 + 0.5f, 0), false);
		Model* indicator = new Model("Indicator.obj", position + glm::vec3(0, 2 * ratio + 0.5f, 0), false);
		Model* slider = new Model("Slider.obj", position + glm::vec3(0, 1.5f, 0), false);
		Button* plus = new Button("PlusButton.obj", position + glm::vec3(0, 3, 0), indicator->GetComponent<TransformComponent>(), 0.2f, 1, onclick);
		Button* minus = new Button("MinusButton.obj", position, indicator->GetComponent<TransformComponent>(), 0.2f, -1, onclick);

		_settingActors.push_back(title);
		_settingActors.push_back(indicator);
		_settingActors.push_back(slider);
		_settingActors.push_back(plus);
		_settingActors.push_back(minus);
	}

	std::vector<Entity*> GetEntities() { return this->_settingActors; }
};

#endif // !SETTINGS_KIT_H

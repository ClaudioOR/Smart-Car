#ifndef SENSOR_H
#define SENSOR_H
#pragma once

#include "glm\glm.hpp"
#include "..\Collision Detection\Collision_Detection.h"

struct Sensor {

	glm::vec4 position;
	glm::vec4 direction;
	float distance;

	Sensor() {
		position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		direction = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		distance = -1.0f;
	}

	Sensor(const glm::vec4 &pos, const glm::vec4 &dir) {
		position = pos;
		direction = dir;
		distance = -1.0f;
	}
};

#endif

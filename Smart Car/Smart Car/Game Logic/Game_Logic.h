#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <map>
#include <iostream>
#include <Windows.h>
#include "../Rendering/IGameObject.h"
#include "../Collision Detection/Collision_Detection.h"
#include "Sensor.h"

#define PI 3.14159265359

class Game_Logic {

public:
	Game_Logic();
	~Game_Logic();

	void Init(int screen_x, int screen_y);
	bool checkBorders(std::vector<Rendering::VertexFormat>& verts, glm::mat4& modelMat);
	bool checkCollision();
	void reset();
	void setModels(std::map<std::string, Rendering::IGameObject*>& gameModelList);
	void setEventHandler(EventHandler& evHndl);
	void compute(float dt);
	void transformObstacles(float dt);
	void turnCar(float angleDec, float dt);
	void proceedInputs(float dt);
	void updateSensors();
	void updateConsole();
	void printTelemetry();

private:
	int screen_x, screen_y, epoche, cursor_x_start, cursor_y_start;
	bool keyboard_control;
	Sensor sensors[5];
	std::map<std::string, Rendering::IGameObject*> models;
	std::vector<Rendering::VertexFormat> border_top, border_left, border_bottom, border_right;
	EventHandler* input;
	HANDLE hConsole;	
	CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
};

#endif
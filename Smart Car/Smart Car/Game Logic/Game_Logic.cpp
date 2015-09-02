#include "Game_Logic.h"

Game_Logic::Game_Logic() {}

Game_Logic::~Game_Logic() {}

/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/
void Game_Logic::compute(float dt) {
	//Check Collisions
	if (this->checkCollision()) {
		/*#####################################*/
		/*### !! React on Collision here !! ###*/
		/*#####################################*/
		std::cout << "Collision!" << std::endl;
		this->reset(); //Reset Car Position
	}
	//Proceed Keyboard Inputs
	this->proceedInputs(dt);
	//Move Obstacles
	this->transformObstacles(dt);
}
/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/

/* If angleDec positive => Turn Car Counter Clock Wise */
/* If angleDec negative => Turn Car Clock Wise		   */
void Game_Logic::turnCar(float angleDec, float dt) {
	float angleRad = angleDec*(PI / 180.0f);
	this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), dt*angleRad, glm::vec3(0.0f, 0.0f, 1.0f)));
}

void Game_Logic::reset() {
	//Reset Cars' Model Matrix to Initial State
	glm::vec3 center(float(screen_x / 2), float(screen_y / 2), 0.0f);
	this->models["car"]->setModelMatrix(glm::translate(glm::mat4(1.), glm::vec3(float(screen_x / 2), float(screen_y / 2), 0.0f)));
}

void Game_Logic::proceedInputs(float dt) {
	//Check Basic Inputs
	if (this->input->getKeyState('2')) //Press 2 to Enabel Keyboard Control
		this->keyboard_control = true;
	if (this->input->getKeyState('1')) //Press 1 to Disable Keyboard Control
		this->keyboard_control = false;
	if (this->input->getKeyState((char)27)) //Exit on ESC
		glutLeaveMainLoop();
	//Check Car Control Input if Keyboard Control is Enabled
	if (this->keyboard_control) {
		//Update Movement
		float movespeed = 0.3;
		float rotspeed = 0.005;
		if (this->input->getKeyState('w'))
			this->models["car"]->setModelMatrix(glm::translate(this->models["car"]->getModelMatrix(), glm::vec3(0, -dt*movespeed, 0)));
		if (this->input->getKeyState('a'))
			this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), -dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f)));
		if (this->input->getKeyState('s'))
			this->models["car"]->setModelMatrix(glm::translate(this->models["car"]->getModelMatrix(), glm::vec3(0, dt*movespeed, 0)));
		if (this->input->getKeyState('d'))
			this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f)));
	}
}

bool Game_Logic::checkCollision() {
	std::vector<Rendering::VertexFormat>& verts = this->models["car"]->getVertices();
	glm::mat4& carMat = this->models["car"]->getModelMatrix();
	//Check Intersection with Car and any Obstacle
	return intersectTriangles(verts, carMat, this->models["obstacle_0"]->getVertices(), this->models["obstacle_0"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_1"]->getVertices(), this->models["obstacle_1"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_2"]->getVertices(), this->models["obstacle_2"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_3"]->getVertices(), this->models["obstacle_3"]->getModelMatrix())
		|| checkBorders(this->models["car"]->getVertices(), this->models["car"]->getModelMatrix());
}

void Game_Logic::transformObstacles(float dt) {
	float rotspeed = 0.000075;
	glm::vec3 center(float(screen_x / 2), float(screen_y / 2), 0.0f);
	//Translate to Origin
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
	//Rotate around Origin
	transform = glm::rotate(transform, -dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f));
	//Translate back to Center
	transform = glm::translate(transform, -center);
	//Apply Transformation on Obstacles
	this->models["obstacle_0"]->setModelMatrix(transform * this->models["obstacle_0"]->getModelMatrix());
	this->models["obstacle_1"]->setModelMatrix(transform * this->models["obstacle_1"]->getModelMatrix());
	this->models["obstacle_2"]->setModelMatrix(transform * this->models["obstacle_2"]->getModelMatrix());
	this->models["obstacle_3"]->setModelMatrix(transform * this->models["obstacle_3"]->getModelMatrix());
}

void Game_Logic::setModels(std::map<std::string, Rendering::IGameObject*>& gameModelList) {
	this->models = gameModelList;
}

void Game_Logic::setEventHandler(EventHandler& evHndl) {
	this->input = &evHndl;
}

void Game_Logic::Init(int screen_x, int screen_y) {
	//Set Screen Size
	this->screen_x = screen_x;
	this->screen_y = screen_y;
	//disable Keyboard Control
	this->keyboard_control = false;
	//Create Border Triangles
	////Top
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), 0.0f, 0.0f, 1.0f)));
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)));
	////Left
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y), 0.0f, 1.0f)));
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)));
	////Bottom
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y), 0.0f, 1.0f)));
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), float(screen_y), 0.0f, 1.0f)));
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y) + 1.0f, 0.0f, 1.0f)));
	////Right
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), 0.0f, 0.0f, 1.0f)));
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), float(screen_y), 0.0f, 1.0f)));
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x) + 1.0f, 0.0f, 0.0f, 1.0f)));
}

bool Game_Logic::checkBorders(std::vector<Rendering::VertexFormat>& verts, glm::mat4& modelMat) {
	//Check Car against Border-Triangles
	return intersectTriangles(verts, modelMat, this->border_top, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_left, glm::mat4(1.0f))
		|| intersectTriangles(verts, modelMat, this->border_bottom, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_right, glm::mat4(1.0f));
}
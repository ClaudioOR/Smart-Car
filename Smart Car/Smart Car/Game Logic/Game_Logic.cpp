#include "Game_Logic.h"

//Macros
#define RAD(A)	(((2.0f * PI) / 360.0f) * A)	

Game_Logic::Game_Logic() {}

Game_Logic::~Game_Logic() {
	delete input;
}

/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/
void Game_Logic::compute(float dt) {
	//Check Collisions
	if (this->checkCollision()) {
		/*#####################################*/
		/*### !! React on Collision here !! ###*/
		/*#####################################*/	
		this->epoche++;
		this->reset(); //Reset Car Position
	}
	//Proceed Keyboard Inputs
	this->proceedInputs(dt);
	//Move Obstacles
	this->transformObstacles(dt);
	//Update Sensor-Data
	this->updateSensors();
	//Update Console Output
	this->updateConsole();
}
/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/

void Game_Logic::Init(int screen_x, int screen_y) {
	/*Set Screen Size*/
	this->screen_x = screen_x;
	this->screen_y = screen_y;
	this->epoche = 1;

	/*disable Keyboard Control*/
	this->keyboard_control = false;

	/*Create Border Triangles*/
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

	/*Init Sensor-Data*/
	//Get Vetices of Car
	std::vector<Rendering::VertexFormat>& carVerts = this->models["car"]->getVertices();
	glm::vec4 p0 = carVerts[0].position;
	glm::vec4 p1 = carVerts[1].position;
	glm::vec4 p2 = carVerts[2].position;
	//Init Center Point c
	glm::vec4 df = (p1 - p0) / 2.0f;
	glm::vec4 ds = (p2 - p0) / 4.0f;
	glm::vec4 c = p0 + df + ds;
	//Init Sensor s0
	glm::vec4 d0 = glm::normalize(p0 - p2);
	sensors[0] = { c, d0 };
	//Init Angles
	float inner_angle = 22.5f;
	float outter_angle = 45.0f;
	//Init Sensor s1
	float deg = RAD(inner_angle);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 d1 = glm::normalize(rotation * d0);
	this->sensors[1] = { c, d1 };
	//Init Sensor s2
	rotation = glm::rotate(glm::mat4(1.0f), -deg, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 d2 = glm::normalize(rotation * d0);
	this->sensors[2] = { c, d2 };
	//Init Sensor s3
	deg = RAD(outter_angle);
	rotation = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 d3 = glm::normalize(rotation * d0);
	this->sensors[3] = { c, d3 };
	//Init Sensor s4
	rotation = glm::rotate(glm::mat4(1.0f), -deg, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 d4 = glm::normalize(rotation * d0);
	this->sensors[4] = { c, d4 };

	/********Init Console Data*********/
	/*Following Code using Windows API*/
	/**********************************/
	//Prepare Console Data for Output
	////Get Handle on Console
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	////Get Screen Buffer Info
	GetConsoleScreenBufferInfo(hConsole, &consoleinfo);
	////Extract Start Cursor Position
	cursor_x_start = consoleinfo.dwCursorPosition.X;
	cursor_y_start = consoleinfo.dwCursorPosition.Y;
	////Print Initial Telemetry
	this->printTelemetry();
	/****************************/
	/*End using Windows API Code*/
	/****************************/
}

void Game_Logic::updateConsole() {
	/**********************************/
	/*Following Code using Windows API*/
	/**********************************/
	//Buffer Current Cursor Position	
	GetConsoleScreenBufferInfo(hConsole, &consoleinfo);
	int cursor_x_temp = consoleinfo.dwCursorPosition.X;
	int cursor_y_temp = consoleinfo.dwCursorPosition.Y;
	//Set Cursor Position to Start
	COORD pos_start = { cursor_x_start, cursor_y_start };
	SetConsoleCursorPosition(hConsole, pos_start);
	//Print Telemetry
	this->printTelemetry();
	//Restore Cursor Position
	COORD pos_temp = { cursor_x_temp, cursor_y_temp };
	SetConsoleCursorPosition(hConsole, pos_temp);
	/****************************/
	/*End using Windows API Code*/
	/****************************/
}

void Game_Logic::printTelemetry() {	
	printf("******************** Start Running Smart - Car Framework *********************\n");
	printf("******************************************************************************");
	printf("\n\n");
	printf("Current Epoche: %i\n", this->epoche);
	printf("Sensor Data   :\n");
	printf("S4 %.3f | S2 %.3f | S0 %.3f | S1 %.3f | S3 %.3f", this->sensors[4].distance, this->sensors[2].distance, this->sensors[0].distance, this->sensors[1].distance, this->sensors[3].distance);
	printf("\n\n");	
	printf("******************************************************************************");
	printf("\n\n");
}

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

bool Game_Logic::checkBorders(std::vector<Rendering::VertexFormat>& verts, glm::mat4& modelMat) {
	//Check Car against Border-Triangles
	return intersectTriangles(verts, modelMat, this->border_top, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_left, glm::mat4(1.0f))
		|| intersectTriangles(verts, modelMat, this->border_bottom, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_right, glm::mat4(1.0f));
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

void Game_Logic::updateSensors() {
	//Transform Sensors in Worldspace
	glm::vec4 d_ws[5];
	float t_nearest, t_temp[8];
	glm::vec4 c_ws = this->models["car"]->getModelMatrix() * this->sensors[0].position;	
	d_ws[0] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[0].direction);
	d_ws[1] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[1].direction);
	d_ws[2] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[2].direction);
	d_ws[3] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[3].direction);
	d_ws[4] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[4].direction);
	//Itetate through all Sensors	
	for (int i = 0; i < 5; i++) {
		//Check Collision: Ray with Border_top Triangle
		t_temp[0] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_top);
		t_temp[1] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_left);
		t_temp[2] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_bottom);
		t_temp[3] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_right);
		t_temp[4] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_0"]->getModelMatrix(), this->models["obstacle_0"]->getVertices());
		t_temp[5] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_1"]->getModelMatrix(), this->models["obstacle_1"]->getVertices());
		t_temp[6] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_2"]->getModelMatrix(), this->models["obstacle_2"]->getVertices());
		t_temp[7] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_3"]->getModelMatrix(), this->models["obstacle_3"]->getVertices());
		//Get nearest Intersection
		t_nearest = FLT_MAX;
		for (int j = 0; j < 8; j++) {
			if (t_temp[j] < t_nearest)
				t_nearest = t_temp[j];
		}
		this->sensors[i].distance = t_nearest;
	}
}
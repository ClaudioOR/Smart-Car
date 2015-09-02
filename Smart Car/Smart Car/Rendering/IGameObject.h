#pragma once
#include <vector>
#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "VertexFormat.h"
#include "../Core/EventHandler.h"
#include "Camera.h"

namespace Rendering {

	class IGameObject {

	public:
		virtual ~IGameObject() = 0;

		virtual void Draw() = 0;
		virtual void Update(float dt) = 0;
		virtual void SetProgram(GLuint shaderName) = 0;
		virtual void Destroy() = 0;
		virtual void setEventHandler(EventHandler* eh) = 0;
		virtual void setCamera(Camera* cam) = 0;
		virtual std::vector<VertexFormat>& getVertices() = 0;
		virtual glm::mat4& getModelMatrix() = 0;
		virtual void setModelMatrix(glm::mat4& model) = 0;

		virtual GLuint GetVao() const = 0;
		virtual const std::vector<GLuint>& GetVbos() const = 0;		

		EventHandler* input;
		Camera* camera;		
	};

	inline IGameObject::~IGameObject() {}
}

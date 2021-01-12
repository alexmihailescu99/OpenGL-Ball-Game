#pragma once
#include <Core/GPU/Mesh.h>
#include <string>
#include <include/glm.h>
#include <Component/SimpleScene.h>
#include "defines.h"
class GameObject : public SimpleScene {
public:
	GLboolean alreadyDone = false;
	// CONSTRUCTOR & DESTRUCTOR
	GameObject(std::string tag, Mesh* mesh, glm::vec3 color, GLfloat xPos = SCREEN_CENTER_WIDTH, GLfloat yPos = SCREEN_CENTER_HEIGHT, GLfloat zPos = 0.5f, GLboolean visible = true,
		GLfloat velX = 0.0f, GLfloat velY = 0.0f, GLfloat velZ = 0.0f);
	~GameObject();
	// CONSTRUCTOR & DESTRUCTOR

	// GETTERS & SETTERS
	std::string getTag() { return this->tag; };
	void setTag(std::string tag) { this->tag = tag; };

	Mesh* getMesh() { return this->mesh; };
	void setMesh(Mesh* mesh) { this->mesh = mesh; };

	std::string getMeshName() { return this->meshName; };
	void setMeshName(std::string meshName) { this->meshName = meshName; };

	glm::mat4 getModelMatrix() { return this->modelMatrix; };
	void setModelMatrix(glm::mat4 modelMatrix) { this->modelMatrix = modelMatrix; };

	GLfloat getXPos() { return this->xPos; };
	GLfloat getYPos() { return this->yPos; };
	GLfloat getZPos() { return this->zPos; };
	void setPos(GLfloat xPos, GLfloat yPos, GLfloat zPos);

	GLfloat getVelX() { return this->velX; };
	void setVelX(GLfloat velX) { this->velX = velX; };

	GLfloat getVelY() { return this->velY; };
	void setVelY(GLfloat velY) { this->velY = velY; };

	GLfloat getVelZ() { return this->velZ; };
	void setVelZ(GLfloat velZ) { this->velZ = velZ; };

	GLboolean isVisible() { return this->visible; };
	void setVisible(GLboolean visible) { this->visible = visible; };

	glm::vec3 getColorType() { return this->colorType; };
	void setColorType(glm::vec3 color) { this->colorType = color; };

	glm::vec3 getShowColor() { return this->showColor; };
	void setShowColor(glm::vec3 color) { this->showColor = color; };
	// GETTERS & SETTERS

	// OBJECT MANIPULATION
	void translate(GLfloat translateX, GLfloat translateY, GLfloat translateZ);
	void scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	void rotateOX(GLfloat radians);
	void rotateOY(GLfloat radians);
	void rotateOZ(GLfloat radians);
	void rotateOX(GLint degrees);
	void rotateOY(GLint degrees);
	void rotateOZ(GLint degrees);
	// OBJECT MANIPULATION

private:
	std::string tag;
	Mesh* mesh;
	std::string meshName;
	glm::mat4 modelMatrix;
	GLfloat xPos;
	GLfloat yPos;
	GLfloat zPos;
	GLfloat velX;
	GLfloat velY;
	GLfloat velZ;
	GLboolean visible;
	glm::vec3 colorType;
	glm::vec3 showColor;
};
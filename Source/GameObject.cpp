#include "GameObject.h"
#include <include/math.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
// Get the mesh name out of the game object name
// E.G. balloon out of balloon01
std::string processMeshName(std::string const& s) {
	std::string::size_type pos = s.find('0');
	return (pos != std::string::npos) ? s.substr(0, pos) : s;
}

GameObject::GameObject(std::string tag, Mesh* mesh, glm::vec3 colorType, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLboolean visible, GLfloat velX, GLfloat velY, GLfloat velZ) {
	this->setTag(tag);
	this->setMeshName(processMeshName(tag));
	this->setMesh(mesh);
	this->setColorType(colorType);
	this->setShowColor(colorType);
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->setModelMatrix(glm::mat4(1));
	//this->translate(xPos, yPos, zPos);
	this->setVisible(visible);
	this->setVelX(velX);
	this->setVelY(velY);
	this->setVelZ(velZ);
};

GameObject::~GameObject() {

};

void GameObject::translate(GLfloat translateX, GLfloat translateY, GLfloat translateZ) {
	glm::mat4 mat = this->modelMatrix *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		translateX, translateY, translateZ, 1.f
	);
	this->xPos = mat[3][0];
	this->yPos = mat[3][1];
	this->zPos = mat[3][2];
}

void GameObject::setPos(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	glm::mat4 mat = glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		xPos, yPos, zPos, 1.f
	);
	this->modelMatrix = mat;
	this->xPos = mat[3][0];
	this->yPos = mat[3][1];
	this->zPos = mat[3][2];
}

void GameObject::scale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ) {
	this->modelMatrix *= glm::mat4(
		scaleX, 0.f, 0.f, 0.f,
		0.f, scaleY, 0.f, 0.f,
		0.f, 0.f, scaleZ, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOX(GLfloat radians) {
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOY(GLfloat radians) {
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		c, 0.f, -s, 0.f,
		0.f, 1.f, 0.f, 0.f,
		s, 0.f, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOZ(GLfloat radians) {
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		c, s, 0.f, 0.f,
		-s, c, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOX(GLint degrees) {
	GLfloat radians = RADIANS((GLfloat)degrees);
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOY(GLint degrees) {
	GLfloat radians = RADIANS((GLfloat)degrees);
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

void GameObject::rotateOZ(GLint degrees) {
	GLfloat radians = RADIANS((GLfloat)degrees);
	GLfloat c = cos(radians);
	GLfloat s = sin(radians);

	this->modelMatrix *= glm::mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, c, s, 0.f,
		0.f, -s, c, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}
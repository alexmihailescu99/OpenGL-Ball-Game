#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <Core/Engine.h>
#define RED glm::vec3(1, 0, 0)
#define GREEN glm::vec3(0, 1, 0)
#define BLUE glm::vec3(0, 0, 1)
#define WHITE glm::vec3(1, 1, 1)
#define ORANGE glm::vec3(1, .5f, 0.f)
#define YELLOW glm::vec3(1, 1, 0)
#define PURPLE glm::vec3(1, 0, 1)
GLfloat gasoline = 100.f;
GLfloat gasolineDecayRate = .05f;
GLfloat gasolineDecayMultiplier = 1.25f;
GLfloat sphereRadius = .5f;
GLfloat platformXLength = 1.f;
GLfloat platformYLength = .1f;
GLfloat platformZLength = 3.5f;
GLboolean freeFall = false;
GLfloat gasPenalty = .1f;
GLfloat gasRecovery = .5f;
GLdouble startTimer = 0.f;
GLboolean maxSpeedPenalty = false;
GLboolean jumpUp = false;
GLboolean jumpDown = false;
GLfloat totalUp = 0.f;
GLfloat gasBarScaleFactor = 1.f;
GLfloat totalScale = 1.f;
glm::mat4 originalModelMatrix;
GLdouble invulnerabilityTimer;
GLboolean invulnerable = true;
Tema2::Tema2() {

}

Tema2::~Tema2() {

}

void Tema2::Init() {
	// CAMERA STUFF
	renderCameraTarget = false;
	camera = new Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	// CAMERA VALUES
	bottom = 0.1f;
	top = 10.0f;
	left = 10.0f;
	right = 0.1f;
	zNear = 0.1f;
	zFar = 100.0f;
	fov = 80.0f;
	// CAMERA VALUES

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	// CAMERA STUFF

	// MESH & GAMEOBJECT CREATION
	Mesh* mesh = new Mesh("platform");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;
	AddGameObject("platform01", mesh, BLUE);
	AddGameObject("platform02", mesh, BLUE);
	AddGameObject("platform03", mesh, BLUE);
	AddGameObject("platform04", mesh, BLUE);
	AddGameObject("platform05", mesh, BLUE);
	AddGameObject("platform06", mesh, BLUE);
	GameObject* box = FindGameObjectByTag("platform01");
	box->setPos(-1.5f, 0.5f, -6.f);
	box->scale(1.0f, 0.1f, 3.5f);

	box = FindGameObjectByTag("platform02");
	box->setPos(0.0f, 0.5f, -6.f);
	box->scale(1.0f, 0.1f, 3.5f);

	box = FindGameObjectByTag("platform03");
	box->setPos(1.5f, 0.5f, -6.f);
	box->scale(1.0f, 0.1f, 3.5f);

	box = FindGameObjectByTag("platform04");
	box->scale(1.0f, 0.1f, 3.5f);
	box->setVisible(false);

	box = FindGameObjectByTag("platform05");
	box->scale(1.0f, 0.1f, 3.5f);
	box->setVisible(false);

	box = FindGameObjectByTag("platform06");
	box->scale(1.0f, 0.1f, 3.5f);
	box->setVisible(false);

	mesh = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	AddGameObject("sphere01", mesh, WHITE);
	GameObject* sphere = FindGameObjectByTag("sphere01");
	sphere->setPos(0.0f, 0.8f, 1.0f);
	sphere->scale(sphereRadius, sphereRadius, sphereRadius);

	mesh = CreateRect("gasolineBar", glm::vec3(0, 0, 0), 1.5f, .1f, glm::vec3(1, 1, 0), true);
	meshes["gasolineBar"] = mesh;
	AddGameObject("gasolineBar01", mesh, GREEN);
	GameObject* gasolineBar = FindGameObjectByTag("gasolineBar01");
	gasolineBar->setPos(-0.8f, 1.75f, 1.f);
	originalModelMatrix = gasolineBar->getModelMatrix();

	AddGameObject("gasolineBar02", mesh, WHITE);
	gasolineBar = FindGameObjectByTag("gasolineBar02");
	gasolineBar->setPos(-0.8f, 1.75f, 1.f);
	// MESH & GAMEOBJECT CREATION

	// SHADER STUFF
	Shader* shader = new Shader("ShaderTema2");
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
	// SHADER STUFF
	srand(time(NULL));
	invulnerabilityTimer = std::clock();
}

void Tema2::FrameStart() {
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void GameOver(std::string message) {
	std::cout << message << std::endl;
	std::cout << "Game over\n";
	exit(1);
}

void Tema2::Update(float deltaTimeSeconds) {
	HandleSphere(deltaTimeSeconds);
	HandlePlatforms(deltaTimeSeconds);
	HandleCollisions();
	RenderGameObjects();
	std::cout << (GLboolean)invulnerable << std::endl;
	if (renderCameraTarget) DrawTPS();
}


void Tema2::FrameEnd() {
	// DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::DrawTPS() {
	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	// I could move this to a separate Game Object and just set its visibility on/off but I'm too lazy atm
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
	RenderSimpleMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
	
}

void Tema2::HandleSphere(GLfloat deltaTimeSeconds) {
	GLfloat invulnerabilityDuration = (std::clock() - startTimer) / (GLdouble)CLOCKS_PER_SEC;
	if (invulnerabilityDuration > 2) {
		invulnerable = false;
	}
	if (gasoline <= 0.f) GameOver("You ran out of gas");
	GameObject* gas = FindGameObjectByTag("gasolineBar01");
	gasBarScaleFactor = gasoline / 100;
	gas->setModelMatrix(originalModelMatrix);
	gas->scale(gasBarScaleFactor, 1.f, 1.f);
	if (maxSpeedPenalty) {
		GLfloat duration = (std::clock() - startTimer) / (GLdouble)CLOCKS_PER_SEC;
		if (duration >= 2) {
			maxSpeedPenalty = false;
			platformTranslationSpeed = initialPlatformTranslationSpeed;
			rotationSpeedMultiplyFactor = 1.5f;
			startTimer = 0.f;
		} else platformTranslationSpeed = platformTranslationSpeedLimit;
	}
	GameObject* sphere = FindGameObjectByTag("sphere01");
	if (freeFall) {
		if (sphere->getYPos() <= -1.5f) GameOver("You fell off the platforms");
		sphere->translate(0.f, -3.f * deltaTimeSeconds, 0.f);
	}
	HandleJump(deltaTimeSeconds);
}

void Tema2::HandleJump(GLfloat deltaTimeSeconds) {
	GameObject* sphere = FindGameObjectByTag("sphere01");
	if (jumpUp) {
		totalUp += 0.1f;
		sphere->translate(0.f, 7.5f * deltaTimeSeconds, 0.f);
		if (totalUp >= 2.5f) {
			jumpUp = false;
			totalUp = 0.f;
			jumpDown = true;
		}
	}
	if (jumpDown) {
		totalUp += 0.1f;
		sphere->translate(0.f, -7.5f * deltaTimeSeconds, 0.f);
		if (totalUp >= 2.5f) {
			jumpUp = false;
			jumpDown = false;
			totalUp = 0.f;
		}
	}
}

void Tema2::HandlePlatforms(GLfloat deltaTimeSeconds) {
	if (freeFall) return;
	gasoline -= gasolineDecayRate;
	for (GameObject* gameObject : this->gameObjects) {
		// First check that we are dealing with a platform
		if (gameObject->getMeshName() == "platform") {
			if (gameObject->getZPos() >= 3.7f) {
				gameObject->setVisible(false);
				gameObject->setPos(0.f, 0.f, 0.f);
			}
			else {
				if (gameObject->isVisible()) {
					gameObject->translate(0.0f, 0.0f, platformTranslationSpeed * deltaTimeSeconds);
				}
			}
			// Then, check if it is out of screen in order to see if we need to translate it or respawn it
			if (gameObject->getZPos() >= 1.4f) {
				// If it is out of screen, check which one is it(left, center or right)
				if (gameObject->getTag() == "platform01") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform04");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(-1.5f, 0.5f, -(GLfloat)randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
				if (gameObject->getTag() == "platform02") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform05");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(0.f, 0.5f, -(GLfloat)randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
				if (gameObject->getTag() == "platform03") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform06");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(1.5f, 0.5f, -(GLfloat)randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
				if (gameObject->getTag() == "platform04") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform01");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(-1.5f, 0.5f, -(GLfloat) randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
				if (gameObject->getTag() == "platform05") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform02");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(0.f, 0.5f, -(GLfloat)randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
				if (gameObject->getTag() == "platform06") {
					if (gameObject->alreadyDone) continue;
					GameObject* correspondingPlatform = FindGameObjectByTag("platform03");
					GLint randZPos = rand() % 40 + 20;
					correspondingPlatform->setPos(1.5f, 0.5f, -(GLfloat)randZPos / 10);
					correspondingPlatform->scale(1.f, 0.1f, 3.5f);
					glm::vec3 randomColor = ChooseRandomColor();
					correspondingPlatform->setColorType(randomColor);
					correspondingPlatform->setShowColor(randomColor);
					correspondingPlatform->setVisible(true);
					gameObject->alreadyDone = true;
					correspondingPlatform->alreadyDone = false;
				}
			}
	
		}
	}
}

void Tema2::HandlePlatformPower(GameObject* platform) {
	if (platform->getColorType() == RED) {
		if (invulnerable) return;
		GameOver("You landed on a red platform");
	}
	if (platform->getColorType() == YELLOW) {
		gasoline -= gasPenalty;
	}
	if (platform->getColorType() == GREEN) {
		gasRecovery = gasolineDecayRate + .1f;
		gasoline += gasRecovery;
		if (gasoline > 100) gasoline = 100;
	}
	if (platform->getColorType() == ORANGE) {
		maxSpeedPenalty = true;
		startTimer = std::clock();
	}
}

GLboolean CheckCollision(GameObject* sphere, GLfloat x, GLfloat y, GLfloat z) {
	GLfloat distance = sqrt((x - sphere->getXPos()) * (x - sphere->getXPos()) + 
							(y - sphere->getYPos()) * (y - sphere->getYPos()) +
							(z - sphere->getZPos()) * (z - sphere->getZPos())
	);
	return distance < sphereRadius;
}
void Tema2::HandleCollisions() {
	GameObject* sphere = FindGameObjectByTag("sphere01");
	GLfloat pointX = sphere->getXPos();
	GLfloat pointY = sphere->getYPos();
	GLfloat pointZ = sphere->getZPos();
	bool hasCollided = false;
	for (GameObject* gameObject : this->gameObjects) {
		if (gameObject->getMeshName() == "platform") {
			GLfloat minX = gameObject->getXPos() - platformXLength / 2;
			GLfloat maxX = gameObject->getXPos() + platformXLength / 2;
			GLfloat minY = gameObject->getYPos() - platformYLength / 2;
			GLfloat maxY = gameObject->getYPos() + platformYLength / 2;
			GLfloat minZ = gameObject->getZPos() - platformZLength / 2;
			GLfloat maxZ = gameObject->getZPos() + platformZLength / 2;
			GLfloat x = std::max(minX, std::min(sphere->getXPos(), maxX));
			GLfloat y = std::max(minY, std::min(sphere->getYPos(), maxY));
			GLfloat z = std::max(minZ, std::min(sphere->getZPos(), maxZ));
			if (CheckCollision(sphere, x, y, z)) {
				gameObject->setShowColor(PURPLE);
				hasCollided = true;
				HandlePlatformPower(gameObject);
			}
		}
	}
	if (!hasCollided && !jumpUp && !jumpDown && !invulnerable) {
		freeFall = true;
	}
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	GLint obj_location = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(obj_location, 1, glm::value_ptr(color));
	// TODO : get shader location for uniform mat4 "Model"

	GLint location = glGetUniformLocation(shader->GetProgramID(), "Model");
	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	// TODO : get shader location for uniform mat4 "View"
	GLint viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	// TODO : get shader location for uniform mat4 "Projection"
	GLint projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


Mesh* Tema2::CreateRect(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

void Tema2::AddGameObject(std::string tag, Mesh* mesh, glm::vec3 colorType, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLboolean visible, GLfloat velX, GLfloat velY, GLfloat velZ) {
	GameObject* gameObject = new GameObject(tag, mesh, colorType, xPos, yPos, zPos, visible, velX, velY, velZ);
	this->gameObjects.push_back(gameObject);
}

GameObject* Tema2::FindGameObjectByTag(std::string tag) {
	for (GameObject* gameObject : this->gameObjects) {
		if (gameObject->getTag() == tag) return gameObject;
	}
	std::cerr << "Could not find " << tag << std::endl;
	return nullptr;
}
void Tema2::RenderGameObject(GameObject* gameObject, Shader* shader) {
	RenderSimpleMesh(meshes[gameObject->getMeshName()], shader, gameObject->getModelMatrix(), gameObject->getShowColor());
}

void Tema2::RenderGameObjects() {
	for (GameObject* gameObject : this->gameObjects) {
		if (gameObject->isVisible()) {
			RenderGameObject(gameObject, shaders["ShaderTema2"]);
		}
	}
}

glm::vec3 Tema2::ChooseRandomColor() {
	GLint randomNumber = rand() % 10;
	switch (randomNumber) {
	case 0:
		return BLUE;
	case 1:
		return YELLOW;
	case 2:
		return ORANGE;
	case 3:
		return GREEN;
	case 4:
		return GREEN;
	case 5:
		return GREEN;
	case 6:
		return RED;
	case 7:
		return BLUE;
	case 8:
		return GREEN;
	case 9:
		return BLUE;
	case 10:
		return YELLOW;
	}
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
	if (window->KeyHold(GLFW_KEY_A)) {
		// Disable user input if the sphere is free falling(user has lost)
		if (freeFall) return;
		GameObject* sphere = FindGameObjectByTag("sphere01");
		if (sphere) {
			// If it's not already on the leftmost platform
			if (sphere->getXPos() > -1.25f) {
				sphere->translate(-5.f * deltaTime, 0.f, 0.f);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		// Disable user input if the sphere is free falling(user has lost)
		if (freeFall) return;
		GameObject* sphere = FindGameObjectByTag("sphere01");
		if (sphere) {
			// If it's not already on the rightmost platform
			if (sphere->getXPos() < 1.25f) {
				sphere->translate(5.f * deltaTime, 0.f, 0.f);
			}
		}
	}
}

void Tema2::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_T) {
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_W) {
		// Disable user input if the sphere is free falling (user has lost) or has landed on an orange platform
		if (freeFall || maxSpeedPenalty) return;
		if (rotationSpeed <= rotationSpeedMax) {
			rotationSpeed *= rotationSpeedMultiplyFactor;
			platformTranslationSpeed *= rotationSpeedMultiplyFactor;
			gasolineDecayRate *= gasolineDecayMultiplier;
		}
	}

	if (key == GLFW_KEY_S) {
		// Disable user input if the sphere is free falling(user has lost) or has landed on an orange platform
		if (freeFall || maxSpeedPenalty) return;
		if (rotationSpeed >= rotationSpeedMin) {
			rotationSpeed /= rotationSpeedMultiplyFactor;
			platformTranslationSpeed /= rotationSpeedMultiplyFactor;
			gasolineDecayRate /= gasolineDecayMultiplier;
		}
	}
	

	if (key == GLFW_KEY_SPACE) {
		// Disable user input if the sphere is free falling(user has lost)
		if (freeFall) return;
		jumpUp = true;
	}
}

void Tema2::OnKeyRelease(int key, int mods) {

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
	
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}

void Tema2::OnWindowResize(int width, int height) {

}



#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "GameObject.h"
#include <vector>
class Tema2 : public SimpleScene {
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void RenderGameObject(GameObject* gameObject, Shader* shader);
	void RenderGameObjects();
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
	// Adds a new gameObject to the world
	void AddGameObject(std::string tag, Mesh* mesh, glm::vec3 colorType, GLfloat xPos = 0.0f, GLfloat yPos = 0.0f, GLfloat zPos = 0.5f, GLboolean visible = true,
		GLfloat velX = 0.0f, GLfloat velY = 0.0f, GLfloat velZ = 0.0f);
	// Returns the required gameObject if found or nullptr if not
	GameObject* FindGameObjectByTag(std::string tag);
	Mesh* CreateRect(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill);
	// Handles sphere stuff
	void HandleSphere(GLfloat deltaTimeSeconds);
	// Handles platform spwawning
	void HandlePlatforms(GLfloat deltaTimeSeconds);
	// Handles collisions
	void HandleCollisions();
	// Handles the jumping mechanic
	void HandleJump(GLfloat deltaTimeSeconds);
	// Handles the special platforms
	void HandlePlatformPower(GameObject* platform);
	// Draws the Third Person Sphere
	void DrawTPS();
	// Chooses a random platform color
	glm::vec3 ChooseRandomColor();
protected:
	std::vector<GameObject*> gameObjects;
	Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	bool projectionType;
	GLfloat right;
	GLfloat left;
	GLfloat bottom;
	GLfloat top;
	GLfloat fov;
	GLfloat zNear, zFar;
	GLfloat rotationSpeed = 2.f;
	GLfloat rotationSpeedMax = 4.f;
	GLfloat rotationSpeedMin = 2.f;
	GLfloat rotationSpeedMultiplyFactor = 1.5f;
	GLfloat initialPlatformTranslationSpeed = .5f;
	GLfloat platformTranslationSpeed = initialPlatformTranslationSpeed;
	GLfloat platformTranslationSpeedLimit = 1.125f;
};

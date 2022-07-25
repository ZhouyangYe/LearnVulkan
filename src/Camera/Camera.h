#pragma once
#include "Core/Common.h"
#include "Renderer/Renderer.h"

#define WALK_SPEED 2.0f
#define SPRINT_SPEED 3.0f
#define CRAWL_SPEED 1.0f
#define MAX_DISTANCE 3.6f

namespace LearnVulkan
{
	class Camera
	{
	public:
		struct WindowSize
		{
			uint32_t width, height;
		};

		struct CameraParam
		{
			WindowSize windowSize;
			float view_distance;
			glm::vec3 spawnLocation;
			float zoom = 0.f;
			glm::vec3 forward{ 1.0f, 0.0f, 0.0f };
			glm::vec3 up{ 0.0f, 0.0f, 1.0f };
		};
		const glm::vec3 WORLD_UP;
		bool freeCamera = true;
		bool cameraMoveDisabled = false;
		bool moveUp = false;
		bool moveDown = false;
		bool moveLeft = false;
		bool moveRight = false;
		bool moveForward = false;
		bool moveBackward = false;

		Camera();
		~Camera();

		const glm::vec3& getEyeCoords();
		const glm::vec3& getCameraCoords();
		const glm::vec3& getForwardVec();
		void Init(CameraParam param);
		void Terminate();
		void Begin();
		void End();
		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		void MoveForward();
		void MoveBackward();
		void Rotate(glm::vec2 delta);
		const bool& cameraMoved();
		const bool& cameraRotated();
		void zoomIn();
		void zoomOut();
		void setCamPos(glm::vec3& pos);
	private:
		glm::mat4 view;
		glm::mat4 proj;
		glm::vec3 eye;
		glm::vec3 cam;
		float zoom;
		glm::vec3 offset;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;
		bool isCameraMoved = false;
		bool isCameraRotated = false;
		void updateCam();
	};
}

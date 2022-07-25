#include "Camera.h"

namespace LearnVulkan
{
	Camera::Camera() : WORLD_UP{ 0.0f, 0.0f, 1.0f } {}

	Camera::~Camera() {}

	void Camera::updateCam()
	{
		cam = eye - offset;
		view = glm::lookAt(cam, eye + forward, up);
	}

	void Camera::zoomIn()
	{
		if (zoom == 0.0f) return;
		zoom -= 0.3;
		if (zoom < 0) zoom = 0;
		offset = zoom * forward;
		updateCam();
	}

	void Camera::zoomOut()
	{
		if (zoom == MAX_DISTANCE) return;
		zoom += 0.3;
		if (zoom > MAX_DISTANCE) zoom = MAX_DISTANCE;
		offset = zoom * forward;
		updateCam();
	}

	void Camera::setCamPos(glm::vec3& pos)
	{
		eye = pos;
		updateCam();
	}

	const glm::vec3& Camera::getCameraCoords()
	{
		return cam;
	}

	const glm::vec3& Camera::getEyeCoords()
	{
		return eye;
	}

	const glm::vec3& Camera::getForwardVec()
	{
		return forward;
	}

	const bool& Camera::cameraMoved()
	{
		return isCameraMoved;
	}

	const bool& Camera::cameraRotated()
	{
		return isCameraRotated;
	}

	void Camera::Init(CameraParam param)
	{
		zoom = param.zoom;
		offset = param.zoom * forward;
		eye = param.spawnLocation;
		cam = eye - offset;
		forward = param.forward;
		up = param.up;
		right = glm::normalize(glm::cross(Camera::forward, Camera::up));
		updateCam();
		proj = glm::perspective(glm::radians(75.0f), float(param.windowSize.width) / float(param.windowSize.height), 0.1f, param.view_distance);
		view = glm::lookAt(cam, eye + forward, up);
		Renderer::setViewTransform(view, proj);
	}

	void Camera::Terminate()
	{}

	void Camera::Begin()
	{
		if (cameraMoveDisabled) return;

		if (moveUp && freeCamera)
			MoveUp();
		if (moveDown && freeCamera)
			MoveDown();
		if (moveLeft)
			MoveLeft();
		if (moveRight)
			MoveRight();
		if (moveForward)
			MoveForward();
		if (moveBackward)
			MoveBackward();

		isCameraMoved = moveUp || moveDown || moveLeft || moveRight || moveForward || moveBackward;
	}

	void Camera::End()
	{
		isCameraMoved = false;
		isCameraRotated = false;

		Renderer::setViewTransform(view, proj);
	}

	void Camera::MoveUp()
	{
		// eye.z += WALK_SPEED * Time::getDeltaTime();
		eye.z += WALK_SPEED;
		updateCam();
	}

	void Camera::MoveDown()
	{
		// eye.z += -WALK_SPEED * Time::getDeltaTime();
		eye.z += -WALK_SPEED;
		updateCam();
	}

	void Camera::MoveLeft()
	{
		// eye += -right * WALK_SPEED * (float)Time::getDeltaTime();
		eye += -right * WALK_SPEED;
		updateCam();
	}

	void Camera::MoveRight()
	{
		// eye += right * WALK_SPEED * (float)Time::getDeltaTime();
		eye += right * WALK_SPEED;
		updateCam();
	}

	void Camera::MoveForward()
	{
		/*float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };*/
		float movement = WALK_SPEED;
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		updateCam();
	}

	void Camera::MoveBackward()
	{
		/*float movement = WALK_SPEED * (float)Time::getDeltaTime();
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += -glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };*/
		float movement = WALK_SPEED;
		glm::vec2 horizontal = glm::normalize(glm::vec2{ forward.x, forward.y });
		eye += -glm::vec3{ horizontal.x * movement, horizontal.y * movement, 0.0f };
		updateCam();
	}

	void Camera::Rotate(glm::vec2 delta)
	{
		/*forward = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP) *
			glm::rotate(-delta.y * (float)Time::getDeltaTime(), right)
		) * forward;

		right = glm::mat3(
			glm::rotate(-delta.x * (float)Time::getDeltaTime(), WORLD_UP)
		) * right;

		up = glm::normalize(glm::cross(right, forward));

		offset = zoom * forward;*/

		forward = glm::mat3(
			glm::rotate(-delta.x, WORLD_UP) *
			glm::rotate(-delta.y, right)
		) * forward;

		right = glm::mat3(
			glm::rotate(-delta.x, WORLD_UP)
		) * right;

		up = glm::normalize(glm::cross(right, forward));

		offset = zoom * forward;

		updateCam();
		isCameraRotated = true;
	}
}

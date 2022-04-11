#include "glpch.h"
#include "PerspectiveCameraController.h"

GLCore::Utils::PerspectiveCameraController::PerspectiveCameraController()
{
}

void GLCore::Utils::PerspectiveCameraController::OnUpdate(Timestep ts)
{
}

void GLCore::Utils::PerspectiveCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseButtonPressedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMousePressed));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseReleased));
	dispatcher.Dispatch<MouseMovedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));

	dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));

}

void GLCore::Utils::PerspectiveCameraController::SetRatio(float ratio)
{
	camera_.SetAspectRatio(ratio);
}


bool GLCore::Utils::PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	// Change FOV
	fov_y_ -= e.GetYOffset() * 0.8f;

	// Set fov boundary
	if (fov_y_ > 98.f) { fov_y_ = 98.f; }
	if (fov_y_ < 2.f) { fov_y_ = 2.f; }

	//std::cout << "ggt" << fov_y_ << std::endl;
	camera_.SetFOVY(fov_y_);

	return false;
}

bool GLCore::Utils::PerspectiveCameraController::OnMousePressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton()==0)
	{
		left_pressed_ = true;
		
	}
	return false;
}

bool GLCore::Utils::PerspectiveCameraController::OnMouseReleased(MouseButtonReleasedEvent& e)
{
	if (e.GetMouseButton()==0)
	{
		left_pressed_ = false;
		left_pressed_pos_recorded_ = false;
	}
	return false;
}

bool GLCore::Utils::PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
{
	if (left_pressed_)
	{
		if (!left_pressed_pos_recorded_)
		{
			left_pressed_pos_recorded_ = true;
			left_pressed_pos_.x = e.GetX();
			left_pressed_pos_.y = e.GetY();

			std::cout << "llk" << left_pressed_pos_.x << ", " << left_pressed_pos_.y << std::endl;
		}
		else
		{
			glm::vec2 dragged;

			dragged.x = e.GetX() - left_pressed_pos_.x;
			dragged.y = e.GetY() - left_pressed_pos_.y;

			std::cout << "jji" << dragged.x << ", " << dragged.y << std::endl;

		}
	}

	
	
	return false;
}



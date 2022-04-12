#pragma once

#include "PerspectiveCamera.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"
#include "GLCore/Events/KeyEvent.h"


namespace GLCore::Utils {


	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetRatio(float ratio);
	
		PerspectiveCamera& GetCamera(){ return camera_; }
		const PerspectiveCamera& GetCamera() const { return camera_; }

		
		

		
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e); // zoom in/out
		bool OnMousePressed(MouseButtonPressedEvent& e); 
		bool OnMouseReleased(MouseButtonReleasedEvent& e); 
		bool OnMouseMoved(MouseMovedEvent& e); // move camera position
		

	private:
		PerspectiveCamera camera_;

		// Pressed Position
		bool				left_pressed_=false;
		bool				left_pressed_pos_recorded_ = false;
		glm::vec2			left_pressed_pos_;
		

	public:
		// 
		float			    aspect_ratio_;
		glm::vec3			camera_position_;
		glm::vec3			camera_stare_;
		float				fov_y_;

		float z_near = 0.01f;
		float z_far = 500.f;

	};

}
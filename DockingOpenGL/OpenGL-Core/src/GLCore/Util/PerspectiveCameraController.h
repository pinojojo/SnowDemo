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
		PerspectiveCameraController(float aspect_ratio);


		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		PerspectiveCamera& GetCamera(){ return camera_; }
		const PerspectiveCamera& GetCamera() const { return camera_; }

		
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e); // zoom in/out
		bool OnWindowResized(WindowResizeEvent& e);  
		bool OnMousePressed(MouseButtonPressedEvent& e); // 
		bool OnKeyPressed(KeyPressedEvent& e); // camera position

	private:
		PerspectiveCamera camera_;
		float aspect_ratio_;
		float camera_position_;
		float fov_y_;
		float z_near;

	

	};

}
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GLCore::Utils {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera();

		void Update(glm::vec3 position,glm::vec3 front,float fov_y,float aspect_ratio);
		void Update(glm::vec3 position, float yaw, float pitch, float roll, float fov_y, float aspect_ratio);

		void SetProjection(float fvo_y,float aspect_ratio,float z_near,float z_far);
		void SetView(glm::vec3 camera_position,glm::vec3 look_at,glm::vec3 camera_up);

		

		
		const glm::mat4& GetProjectionMatrix() const { return mat_projection_; }
		const glm::mat4& GetViewMatrix() const { return mat_view_; }
		const glm::mat4& GetViewProjectionMatrix() const { return mat_view_projection_; }


	private:
		// matrices
		glm::mat4 mat_projection_;
		glm::mat4 mat_view_;
		glm::mat4 mat_view_projection_;

		// camera status
		glm::vec3 position_;
		glm::vec3 front_;
		glm::vec3 up_;
		glm::vec3 right_;
		glm::vec3 world_up_;

		float yaw_;
		float pitch_;
		float roll_;
		
		float fov_y_;
		float aspect_ratio_;
		float z_near_=0.01;
		float z_far_=500.f;
	};


}





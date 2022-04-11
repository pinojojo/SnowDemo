#include "glpch.h"
#include "PerspectiveCamera.h"



GLCore::Utils::PerspectiveCamera::PerspectiveCamera()
{
}

void GLCore::Utils::PerspectiveCamera::Update(glm::vec3 position, glm::vec3 front, float fov_y, float aspect_ratio)
{
	position_ = position;
	front_ = glm::normalize(front);
	fov_y_ = fov_y;
	aspect_ratio_ = aspect_ratio;

	// calc vec
	world_up_ = glm::vec3(0, 0, 1);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));

	// v p matrix
	mat_view_ = glm::lookAt(position_, position_ + front_, up_);
	//mat_view_ = glm::mat4(1.f);
	//mat_view_ = glm::translate(mat_view_, glm::vec3(0.0f, 0.0f, -3.0f));
	mat_projection_ = glm::perspective(fov_y_, aspect_ratio_, z_near_, z_far_);
	mat_view_projection_ = mat_projection_ * mat_view_;


}

void GLCore::Utils::PerspectiveCamera::SetAspectRatio(float ratio)
{
	aspect_ratio_ = ratio;
}

void GLCore::Utils::PerspectiveCamera::SetFOVY(float fovy)
{
	fov_y_ = fovy;
}

void GLCore::Utils::PerspectiveCamera::SetPosition(glm::vec3 posiiton)
{
	position_ = posiiton;
}

void GLCore::Utils::PerspectiveCamera::SetStare(glm::vec3 stare)
{
	stare_ = stare;
}



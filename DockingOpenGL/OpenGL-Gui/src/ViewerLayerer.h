#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/KeyEvent.h"
#include "GLCore/Events/MouseEvent.h"

#include "../../SnowRenderer/ModelRenderer.h"



class ViewerLayerer : public GLCore::ImGuiLayer 
{
public:
	ViewerLayerer();
	virtual ~ViewerLayerer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;



private:

	bool OnMouseButtonPressedEvent(GLCore::MouseButtonPressedEvent& e);
	bool OnMouseMovedEvent(GLCore::MouseMovedEvent& e);


	void CreateVAO();
	void CreateFBO(int width, int height);
	void ResizeFBO(int width, int height);
	void CreateShader();
	void UpdateShader();
	void DrawCube(int width, int height);

	void CreateExampleCubeVAO();


private:
	int frame_loop_id_ = 0;
	bool viewer_focused_;
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint fbo_ = 0;
	GLuint aa_fbo_=0;
	GLuint tex_ = 0;
	int fbo_width_ = 1000;
	int fbo_height_ = 1000;
	int viewer_panel_width_ = 1000;
	int viewer_panel_height_ = 1000;

	renderer::ModelRenderer model_renderer_;
	bool wire_mode_=false;
	bool aa_mode_ = false;
	
	GLCore::Utils::Shader* shader_;

	GLCore::Utils::PerspectiveCamera camera_;
	GLCore::Utils::PerspectiveCameraController camera_controller_;
	glm::vec3 camera_position_ = glm::vec3(0,100,40);
	glm::vec3 camera_stare_ = glm::vec3(0);
	glm::vec3 light_position_=glm::vec3(-5.0f,-5.0f,100.f);
	glm::vec3 light_color_ = glm::vec3(0.9f, 0.9f, 0.9f);
	glm::vec3 object_color_=glm::vec3(0.2f,0.8f,0.3f);
	float fovy_=45;
	glm::vec3 front_ = glm::vec3(1, 0, 0);


};

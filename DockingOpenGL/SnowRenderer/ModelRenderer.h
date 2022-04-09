#pragma once
#include "Model.h"

namespace renderer
{
	class ModelRenderer
	{
	public:
		ModelRenderer();

		void Draw(bool wire_mode);
		GLuint GetRendered();

		void Init(std::string path);
		void SetCamera(glm::vec3 camera_position, glm::vec3 camera_stare, float fovy);
		void SetModel(glm::mat4 model);
		void SetAspect(int width, int height);
		void SetLight(glm::vec3 light_position, glm::vec3 light_color);
		void SetObjectColor(glm::vec3 color);
		void SetWireMode(bool wire_mode);
		void SetAAMode(bool aa_mode);

		
	private:
		void SetupShader();
		void ResizeFBO(int width, int height);// do size adaptation
		void CreateFBO();
		
		
	private:
		Model* model_;
		GLCore::Utils::Shader* phong_shader_;
		int width_, height_ = 800;
		GLuint fbo_=0;

		bool anti_aliased_mode_ = false;
		GLuint aa_fbo_ = 0;
		GLuint aa_tex_ = 0;

		GLuint result_tex_=0;

		glm::mat4 model_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 projection_matrix_;
		glm::mat3 normal_matrix_;



	};

}



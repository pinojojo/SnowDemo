#pragma once


#include <GLCore.h>
#include <GLCoreUtils.h>


namespace renderer
{



	class Polyline
	{
	public:
		Polyline(std::vector<glm::vec2>& points, float z_value);

		void Draw(GLCore::Utils::Shader* shader);
		
	public:
		std::vector<glm::vec3> points_;
		std::vector<unsigned int> indices_;
		bool is_hole = false;
	
		glm::vec3 color_hole_ = glm::vec3(1, 0, 0);
		glm::vec3 color_contour_ = glm::vec3(0, 1, 0);


	private:
		void MakeVAO();


		GLuint line_vao_;// lines
		GLuint line_vbo_;
		GLuint line_ebo_;


		GLuint triangle_vao_;// triangulated 
		
		


	};


}

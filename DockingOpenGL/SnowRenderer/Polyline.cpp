#include "Polyline.h"

renderer::Polyline::Polyline(std::vector<glm::vec2>& points, float z_value)
{
	for (auto& p:points)
	{
		points_.push_back(glm::vec3(p.x, p.y, z_value));
	}

	MakeVAO();
}

void renderer::Polyline::Draw(GLCore::Utils::Shader* shader)
{
	// Set type in Shader
	shader->setBool("is_hole", is_hole);
	shader->setVec3("hole_color", color_hole_);
	shader->setVec3("contour_color", color_contour_);

	glBindVertexArray(line_vao_);
	glDrawElements(GL_LINES, indices_.size(), GL_UNSIGNED_INT, 0);
}

void renderer::Polyline::MakeVAO()
{

	// Create indices
	for (size_t i = 0; i < points_.size(); i++)
	{
		if (i!=points_.size()-1)
		{
			indices_.push_back(i);
			indices_.push_back(i+1);
		}
		else
		{
			indices_.push_back(i);
			indices_.push_back(0);
		}
	}



	// Create OpenGL Buffers
	glGenVertexArrays(1, &line_vao_);
	glGenBuffers(1, &line_vbo_);
	glGenBuffers(1,&line_ebo_);

	glBindVertexArray(line_vao_);

	// Create VBO buffer object to buffer data for Current VAO
	glBindBuffer(GL_ARRAY_BUFFER, line_vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points_), &points_[0].x, GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), &indices_[0], GL_STATIC_DRAW);

	// VAO Attrib Pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glBindVertexArray(0);

}

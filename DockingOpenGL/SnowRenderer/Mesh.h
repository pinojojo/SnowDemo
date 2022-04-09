#pragma once


#define STL_READER_NO_EXCEPTIONS
#include "stl_reader.h"

#include <GLCore.h>
#include <GLCoreUtils.h>

namespace renderer
{
	using namespace std;
	struct Vertex
	{
		// position
		glm::vec3 position=glm::vec3(0);
		// normal
		glm::vec3 normal=glm::vec3(0);
	};


	class Mesh
	{
	public:
		vector<Vertex> vertices_;
		vector<unsigned int> indices_;
		GLuint vao_;

	public:
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
		void Draw(GLuint program_id, bool wire_mode);
	
	private:
		GLuint vbo_, ebo_ = 0;
		// Setup OpenGL buffers from data
		void SetupMesh();

	};

}
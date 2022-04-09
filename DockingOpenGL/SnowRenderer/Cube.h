#pragma once

#include "Mesh.h"

namespace renderer 
{
	
	void GetCube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{ 
		vertices.clear();
		
		float cube_size = 3.0;

		Vertex vert;
		{
			// top 
			vert.position = glm::vec3(-cube_size / 2.f, -cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);
			vert.position = glm::vec3(-cube_size / 2.f, cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);
			vert.position = glm::vec3(-cube_size / 2.f, -cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, -cube_size / 2.f, cube_size / 2.f); vert.normal = glm::vec3(0, 0, 1); vertices.push_back(vert);

			//bottom 
			vert.position = glm::vec3(-cube_size / 2.f, -cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, -cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);
			vert.position = glm::vec3(-cube_size / 2.f, cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);
			vert.position = glm::vec3(-cube_size / 2.f, cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, -cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);
			vert.position = glm::vec3(cube_size / 2.f, cube_size / 2.f, -cube_size / 2.f); vert.normal = glm::vec3(0, 0, -1); vertices.push_back(vert);

			

		
		}
		{
			indices.clear();

			// use cw winding for face culling if neeeded.
			for (int i = 0; i < vertices.size(); i++)
			{
				indices.push_back(i);
			}





		}
		
	

	}



}
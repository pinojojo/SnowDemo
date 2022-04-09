#include "Model.h"

#include "Cube.h"



renderer::Model::Model(string path)
{
	LoadModel(path);
}

renderer::Model::Model()
{
	// use default cube as model
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	GetCube(vertices, indices);
	Mesh cube_mesh(vertices, indices);
	meshes_.push_back(cube_mesh);
	
}

void renderer::Model::Draw(GLuint program_id, bool wire_mode)
{
	for (unsigned int i = 0; i < meshes_.size(); i++)
		meshes_[i].Draw(program_id, wire_mode);
}



void renderer::Model::LoadModel(string path)
{

	stl_reader::StlMesh <float, unsigned int> mesh(path);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<float> coords, normals;
	std::vector<unsigned int> tris, solids;

	stl_reader::ReadStlFile(path.c_str(), coords, normals, tris, solids);
	const size_t numTris = tris.size() / 3;
	for (size_t itri = 0; itri < numTris; ++itri) {
		//std::cout << "coordinates of triangle " << itri << ": ";
		glm::vec3 point[3];
		for (size_t icorner = 0; icorner < 3; ++icorner) {
			float* c = &coords[3 * tris[3 * itri + icorner]];
			//std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
			
			// three points
			point[icorner].x = c[0];
			point[icorner].y = c[1];
			point[icorner].z = c[2];
		}
		//std::cout << std::endl;

		float* n = &normals[3 * itri];
		//std::cout << "normal of triangle " << itri << ": "
		//	<< "(" << n[0] << ", " << n[1] << ", " << n[2] << ")\n";

		glm::vec3 normal;
		normal.x = n[0];
		normal.y = n[1];
		normal.z = n[2];

		if (glm::length(normal) < 0.001f)
		{
			normal=glm::triangleNormal(point[0], point[1], point[2]);
			//std::cout << "normal of triangle " << itri << ": "
			//	<< "(" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
		}

		
		for (size_t i = 0; i < 3; i++)
		{
			Vertex vertex;
			vertex.position = point[i];
			vertex.normal = normal;
			vertices.push_back(vertex);
		}
	}

	// indices
	for (size_t i = 0; i < vertices.size(); i++)
	{
		indices.push_back(i);
	}

	std::cout << "load points: " << indices.size() << std::endl;

	meshes_.push_back(Mesh(vertices, indices));

}


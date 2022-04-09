#pragma once
#define STL_READER_NO_EXCEPTIONS
#include "stl_reader.h"

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "Mesh.h"

namespace renderer 
{
	using namespace std;
	class Model
	{
	public:
		vector<Mesh> meshes_;
	public:
		Model(string path);
		Model();
		void Draw(GLuint program_id, bool wire_mode);
		
	private:
		
		void LoadModel(string path);

	

	};


}

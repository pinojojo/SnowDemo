#pragma once


#include <GLCore.h>
#include <GLCoreUtils.h>


namespace parser
{
	using namespace GLCore;

	struct Polygon 
	{
		std::vector<glm::vec2> points;
		bool is_hole;

		std::string raw_string;

	};
	

	struct Layer 
	{
		std::vector<Polygon> polygons;
		float z_value;
		std::string raw_string;
		std::string name;


	};


	struct SlicedModel
	{
		std::vector<Layer> layers;
		std::string path;

	};


}
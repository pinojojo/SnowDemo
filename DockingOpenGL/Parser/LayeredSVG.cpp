#include "LayeredSVG.h"

#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

void parser::LayeredSVG::ParseModel(std::string path_svg)
{
	using namespace std;
	std::string line;
	std::ifstream in(path_svg);

	if (!in.good())
	{
		cout << "ERROR: svg path cannot open!" << path_svg << endl;
		return;
	}

	bool layer_tag = false;
	int layer_num = 0;
	Layer temp_layer;


	// Get All Layer strings
	while (getline(in, line))
	{
		// find layer start
		if (line.find("<g") != std::string::npos) {
			layer_num++;
			layer_tag = true;
			temp_layer.raw_string += line;
		}
		else
		{
			// find layer end
			if (line.find("</g") != std::string::npos) {
				temp_layer.raw_string += line;

				model.layers.push_back(temp_layer);
				temp_layer.raw_string = "";
			}
			else
			{
				if (layer_tag)
				{
					temp_layer.raw_string += line;
				}
			}
		}
	}

	// Parse all layers
	for (auto& l:model.layers)
	{
		ParseLayer(l);
	}

}

void parser::LayeredSVG::ParseLayer(Layer& layer)
{
	using namespace std;
	// 1. name
	if (int namePosStart = layer.raw_string.find("id=\""))
	{
		if (int namePosEnd = layer.raw_string.find("\"", namePosStart + 4))
		{
			// std::cout << layer.layer_string.substr(namePosStart+4, namePosEnd - namePosStart-4);
			layer.name = layer.raw_string.substr(namePosStart + 4, namePosEnd - namePosStart - 4);

		}
	}

	// 2.  z value
	if (int namePosStart = layer.raw_string.find("z=\""))
	{
		if (int namePosEnd = layer.raw_string.find("\"", namePosStart + 3))
		{
			layer.z_value = std::stof(layer.raw_string.substr(namePosStart + 3, namePosEnd - namePosStart - 3));
		}
	}

	//  3. polygons 
	{
		vector<size_t> startPos;
		vector<size_t> endPos;

		string toSearch = "<polygon";
		size_t pos = layer.raw_string.find(toSearch);

		while (pos != std::string::npos)
		{
			// Add position to the vector
			startPos.push_back(pos + toSearch.size());

			size_t ended = layer.raw_string.find("/>", pos + toSearch.size());
			if (ended != string::npos)
			{
				endPos.push_back(ended);
			}

			// Get the next occurrence from the current position
			pos = layer.raw_string.find(toSearch, pos + toSearch.size());
		}

		for (size_t polygonId = 0; polygonId < startPos.size(); polygonId++)
		{
			Polygon temp_polygon;
			temp_polygon.raw_string = layer.raw_string.substr(startPos[polygonId], endPos[polygonId] - startPos[polygonId]);
			ParsePolygon(temp_polygon);
			layer.polygons.push_back(temp_polygon);
		}
	}


}

void parser::LayeredSVG::ParsePolygon(Polygon& polygon)
{
	using namespace std;
	// 1. points
	{
		string toSearch = "points=\"";
		size_t pointsPos = polygon.raw_string.find(toSearch);

		if (pointsPos != std::string::npos)
		{
			string endToSearch = "\"";
			size_t endedPos = polygon.raw_string.find(endToSearch, pointsPos + toSearch.size());
			if (endedPos != std::string::npos)
			{
				string pointsString = polygon.raw_string.substr(pointsPos + toSearch.size(), endedPos - (pointsPos + toSearch.size()));

				// parse points string 
				{
					glm::vec2 point;
					size_t lastWhiteSpcaePos = 0;
					size_t whiteSpacePos = 0;

					string toSearch = " ";
					whiteSpacePos = pointsString.find(toSearch);

					polygon.points.clear();

					while (whiteSpacePos != std::string::npos)
					{
						string aPointString = pointsString.substr(lastWhiteSpcaePos, whiteSpacePos - lastWhiteSpcaePos);
						lastWhiteSpcaePos = whiteSpacePos + toSearch.size();
						whiteSpacePos = pointsString.find(toSearch, lastWhiteSpcaePos);

						point = glm::vec2(0);
						size_t commaPos = aPointString.find(",");
						point.x = std::stof(aPointString.substr(0, commaPos));
						point.y = std::stof(aPointString.substr(commaPos + 1));

						polygon.points.push_back(point);
					}
				}

			}
		}
	}


	// 2. type 
	{
		string toSearch = "type=\"";
		size_t pointsPos = polygon.raw_string.find(toSearch);

		if (pointsPos != std::string::npos)
		{
			string endToSearch = "\"";
			size_t endedPos = polygon.raw_string.find(endToSearch, pointsPos + toSearch.size());
			if (endedPos != std::string::npos)
			{
				string type_string = polygon.raw_string.substr(pointsPos + toSearch.size(), endedPos - (pointsPos + toSearch.size()));

				if (type_string == "contour")
				{
					polygon.is_hole = false;
				}
				if (type_string == "hole")
				{
					polygon.is_hole = true;
				}
			}
		}
	}
}


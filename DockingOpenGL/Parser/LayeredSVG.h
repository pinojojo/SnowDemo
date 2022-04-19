#pragma once

#include "ParserType.h"

namespace parser {
	class LayeredSVG
	{
	public:
		void ParseModel(std::string path_svg);
	private:
		void ParseLayer(Layer& layer);
		void ParsePolygon(Polygon& polygon);
		
		SlicedModel model;
	};

}



#pragma once
#include "Slice.h"
#include <string>
#include "../Parser/LayeredSVG.h"
namespace renderer
{
	class SlicedModel
	{
	public:

		void Init(std::string path);


	private:
		std::vector<Slice> slices_;
		parser::LayeredSVG sliced_model_parser_;

	};

}



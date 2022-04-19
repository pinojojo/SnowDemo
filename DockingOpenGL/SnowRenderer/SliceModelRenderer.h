#pragma once

#include "SlicedModel.h"


namespace renderer 
{

	class SliceModelRenderer
	{
	public:
		SliceModelRenderer();

		void Draw();


	private:
	
		GLCore::Utils::Shader* polyline_shader_;

		SlicedModel sliced_model_;

	};


}

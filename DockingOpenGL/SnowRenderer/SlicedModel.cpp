#include "SlicedModel.h"

void renderer::SlicedModel::Init(std::string path)
{
	sliced_model_parser_.ParseModel(path);
}

#include "SliceModelRenderer.h"

renderer::SliceModelRenderer::SliceModelRenderer()
{
	sliced_model_.Init(".svg");


	polyline_shader_->FromGLSLTextFiles("assets/shaders/polyline.vert", "assets/shaders/polyline.frag");

}

void renderer::SliceModelRenderer::Draw()
{
}

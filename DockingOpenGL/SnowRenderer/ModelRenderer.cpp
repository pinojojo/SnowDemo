#include "ModelRenderer.h"

renderer::ModelRenderer::ModelRenderer()
{
}

void renderer::ModelRenderer::Draw(bool wire_mode)
{

   
    if (anti_aliased_mode_)
    {
        //std::cout << "A" << std::endl;
        // multi sampling
        glBindFramebuffer(GL_FRAMEBUFFER, aa_fbo_);
        glClearColor((float)39 / 255, (float)39 / 255, (float)42 / 255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, width_, height_);
        /*Draw Area*/
        {
            model_->Draw(phong_shader_->GetRendererID(), wire_mode);
        }

        // down sampling
        glBindFramebuffer(GL_READ_FRAMEBUFFER, aa_fbo_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
        glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    else 
    {
        //std::cout << "B" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor((float)39 / 255, (float)39 / 255, (float)42 / 255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width_, height_);

        /*Draw Area*/
        {
            model_->Draw(phong_shader_->GetRendererID(), wire_mode);
        }


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
}

GLuint renderer::ModelRenderer::GetRendered()
{
    

    return result_tex_;

}

void renderer::ModelRenderer::Init(std::string path)
{
    
    ResizeFBO(800,800);
    SetupShader();

    model_ = new Model(path);
}

void renderer::ModelRenderer::SetCamera(glm::vec3 camera_position, glm::vec3 camera_stare, float fovy)
{
    
    projection_matrix_= glm::perspective(fovy, (float)width_ / height_, 0.1f, 500.f);

    view_matrix_ = glm::lookAt(camera_position, camera_stare, glm::vec3(0, 0, 1));

    phong_shader_->Use();
    phong_shader_->setMat4("view", view_matrix_);
    phong_shader_->setMat4("projection", projection_matrix_);
    
}

void renderer::ModelRenderer::SetModel(glm::mat4 model)
{
    this->model_matrix_ = model;
    phong_shader_->Use();
    phong_shader_->setMat4("model", model);

}

void renderer::ModelRenderer::SetAspect(int width, int height)
{
    ResizeFBO(width, height);
}

void renderer::ModelRenderer::SetLight(glm::vec3 light_position, glm::vec3 light_color)
{
    phong_shader_->Use();
    phong_shader_->setVec3("lightPos", light_position);
    phong_shader_->setVec3("lightColor", light_color);
}

void renderer::ModelRenderer::SetObjectColor(glm::vec3 color)
{
    phong_shader_->Use();
    phong_shader_->setVec3("objectColor", color);
}

void renderer::ModelRenderer::SetWireMode(bool wire_mode)
{
    if (wire_mode)
    {

    }
    else 
    {

    }
}

void renderer::ModelRenderer::SetAAMode(bool aa_mode)
{
    anti_aliased_mode_ = aa_mode;
    
    
}

void renderer::ModelRenderer::SetupShader()
{
   phong_shader_ = GLCore::Utils::Shader::FromGLSLTextFiles( "assets/shaders/phong.vert","assets/shaders/phong.frag");

}

void renderer::ModelRenderer::ResizeFBO(int width, int height)
{
    if ((width_!=width)||(height_!=height))
    {
        width_ = width;
        height_ = height;
        if (fbo_)
        {
			glDeleteFramebuffers(1, &fbo_);
			glDeleteTextures(1, &result_tex_);

			glDeleteFramebuffers(1, &aa_fbo_);
			glDeleteTextures(1, &aa_tex_);
            
        }
        CreateFBO();
    }
    else
    {
        // do nothing
    }

}

void renderer::ModelRenderer::CreateFBO()
{
    std::cout << "create fbo" << std::endl;

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    // create a color attachment texture
    glGenTextures(1, &result_tex_);
    glBindTexture(GL_TEXTURE_2D, fbo_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, result_tex_, 0);

    // depth buffer attchment
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: model renderer framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //----------------------------------------------------------------------------------------
   

	glGenFramebuffers(1, &aa_fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, aa_fbo_);
	// create a multisampled color attachment texture

	glGenTextures(1, &aa_tex_);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, aa_tex_);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width_, height_, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, aa_tex_, 0);
	// create a (also multisampled) renderbuffer object for depth and stencil attachments
	unsigned int rbo_aa;
	glGenRenderbuffers(1, &rbo_aa);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_aa);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width_, height_);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_aa);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: model renderer aa Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

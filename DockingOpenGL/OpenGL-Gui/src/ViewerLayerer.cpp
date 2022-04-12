#include "ViewerLayerer.h"

using namespace GLCore;
using namespace GLCore::Utils;

ViewerLayerer::ViewerLayerer()
{
}

ViewerLayerer::~ViewerLayerer()
{
}

void ViewerLayerer::OnAttach()
{
    CreateVAO();
    CreateFBO(fbo_width_, fbo_height_);
    CreateShader();

    camera_controller_.fov_y_ = 45.f;
    
    model_renderer_.Init("../Resources/models/UMS5_Fresnel_lens.stl");
}

void ViewerLayerer::OnDetach()
{
}

void ViewerLayerer::OnEvent(GLCore::Event& event)
{
    if (viewer_focused_)
    {
        camera_controller_.OnEvent(event);

    }
    
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(GLCORE_BIND_EVENT_FN(ViewerLayerer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(GLCORE_BIND_EVENT_FN(ViewerLayerer::OnMouseMovedEvent));
}

void ViewerLayerer::OnUpdate(GLCore::Timestep ts)
{
    if (frame_loop_id_++>1000)
    {
        frame_loop_id_ = 0;
    }

    // Updating renderer and get new result 
    model_renderer_.SetAspect(viewer_panel_width_, viewer_panel_height_);
    model_renderer_.SetCamera(camera_position_, camera_stare_, glm::radians(fovy_));
    model_renderer_.SetLight(light_position_, light_color_);
    model_renderer_.SetModel(glm::mat4(1.f));
    model_renderer_.SetObjectColor(object_color_);
    model_renderer_.SetWireMode(wire_mode_);
    model_renderer_.SetAAMode(aa_mode_);

    model_renderer_.Draw(wire_mode_);
   

}

void ViewerLayerer::OnImGuiRender()
{
    // controler panel for viewer
    {
        ImGui::Begin("view settings");

        // camera
        static float vec4f[4] = { 5.0f, 5.0f, 5.0f, 0.44f };
        if (ImGui::DragFloat3("camera position", vec4f, 0.1f, -10.f, 100.0f))
        {
            camera_position_.x = vec4f[0];
            camera_position_.y = vec4f[1];
            camera_position_.z = vec4f[2];
            
        }

        static float camera_stare[3] = { 0.0f, 0.0f, 0.0f };
        if (ImGui::DragFloat3("camera stare", camera_stare, 0.1f, -10.f, 10.0f))
        {

        }

        static float fovy = 45;
        if (ImGui::DragFloat("camera fovy", &fovy, 0.1, 0.0, 90.f))
        {
            fovy_ = fovy;
        }


        ImGui::Separator();
        ImGui::LabelText("Light Attributes", "Value");


        // light
        static float light_position[3] = { -5.0f, 3.0f, 5.0f };
        if (ImGui::DragFloat3("light position", light_position, 0.1f, -10.f, 10.0f))
        {

        }

        static float light_color[3] = { 1.0f, 0.5f, 0.6f };
        if (ImGui::DragFloat3("light color", light_color, 0.01f, 0.f, 1.0f))
        {

        }

        static float object_color[3] = { 1.0f, 0.5f, 0.6f };
        if (ImGui::DragFloat3("object color", object_color, 0.01f, 0.f, 1.0f))
        {
            object_color_.x = object_color[0];
            object_color_.y = object_color[1];
            object_color_.z = object_color[2];
        }


        static bool wire_mode=false;
        if (ImGui::Checkbox("Wire Mode", &wire_mode))
        {
            wire_mode_ = wire_mode;
        }
       
        static bool aa_mode = false;
        if (ImGui::Checkbox("Anti-Aliased", &aa_mode))
        {
            
            aa_mode_ = aa_mode;
           
            
        }

       


        ImGui::End();
    }


    // Imgui demo window
    ImGui::ShowDemoWindow();

   
	//viewer window 
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.,0 });
        ImGui::Begin("viewer");

        if (ImGui::IsWindowFocused())
        {
            viewer_focused_ = true;
        }
        else
        {
            viewer_focused_ = false;

        } 
        

        // get panel size
        ImVec2 viewer_panel_size = ImGui::GetContentRegionAvail();
        viewer_panel_width_ = viewer_panel_size.x;
        viewer_panel_height_ = viewer_panel_size.y;
        //ImGui::Image((void*)(intptr_t)tex_, ImVec2(fbo_width_, fbo_height_), ImVec2{ 0,1 }, ImVec2{ 1, 0 });
        ImGui::Image((void*)(intptr_t)model_renderer_.GetRendered(), ImVec2(viewer_panel_width_, viewer_panel_height_), ImVec2{ 0,1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();  
    }
   
}


//
//void ViewerLayerer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
//{
//}

bool ViewerLayerer::OnMouseButtonPressedEvent(GLCore::MouseButtonPressedEvent& e)
{
    //std::cout <<"aas" << e.GetMouseButton() << std::endl;
    return false;
}

bool ViewerLayerer::OnMouseMovedEvent(GLCore::MouseMovedEvent& e)
{
    //std::cout << "ffg" << e.GetX() << ", " << e.GetY() << std::endl;
    return false;
}

void ViewerLayerer::CreateVAO()
{
    CreateExampleCubeVAO();

}

void ViewerLayerer::CreateFBO(int width, int height)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    // create a color attachment texture
    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, fbo_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0);  

    // depth buffer attchment
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: infill framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewerLayerer::ResizeFBO(int width, int height)
{
    
    // clear old fbo
    glDeleteTextures(1, &tex_);
    glDeleteFramebuffers(1, &fbo_);

    // new
    CreateFBO(width, height);
    fbo_width_ = width;
    fbo_height_ = height;
}



void ViewerLayerer::CreateShader()
{

    shader_ = Shader::FromGLSLTextFiles(
        "assets/shaders/viewer.vert.glsl",
        "assets/shaders/viewer.frag.glsl"
    );
}

void ViewerLayerer::UpdateShader()
{

    camera_.Update(camera_position_, glm::vec3(0) - camera_position_, glm::radians(fovy_), (float)fbo_width_ / (float)fbo_height_);

    glUseProgram(shader_->GetRendererID());

   
    glm::mat4 model = glm::mat4(1.0);
    //model = glm::rotate(model, glm::radians(0.01f* frame_loop_id_), glm::vec3(0, 0, 1));
    glm::mat4 mvp = camera_.GetViewProjectionMatrix() * model;
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "mvp"), 1, GL_FALSE, &mvp[0][0]);

}



void ViewerLayerer::DrawCube(int width, int height)
{
    if ((width!=fbo_width_)||(height!=fbo_height_))
    {
        ResizeFBO(width, height);
    }

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    ////glFrontFace(GL_CCW);
   
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, fbo_width_, fbo_height_);

   


    shader_->Use();
    UpdateShader();
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //model->Draw(shader_->GetRendererID());


    glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void ViewerLayerer::CreateExampleCubeVAO()
{
    float vertices[] = {
        //
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,0.0f,0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 1.0f,0.0f,0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 1.0f,0.0f,0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,0.0f,0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,
        //                        
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,
         0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,1.0f,0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,1.0f,0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,
        ////                               
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,0.0f,1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,
        // //                             
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,1.0f,0.0f,
         0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 1.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,1.0f,0.0f,
        // //                 
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,0.0f,1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 1.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,0.0f,1.0f,
        ////                   
        -0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,1.0f,1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,
    };

    // create vao
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // face id 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    

}


#include "SliceViewer.h"
using namespace GLCore;
using namespace GLCore::Utils;

SliceViewer::SliceViewer()
{
}

SliceViewer::~SliceViewer()
{
}

void SliceViewer::OnAttach()
{
}

void SliceViewer::OnDetach()
{
}

void SliceViewer::OnEvent(GLCore::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{

			std::cout << "slice viewer pressed" << e.GetMouseButton() << std::endl;
			return false;
		});
}

void SliceViewer::OnUpdate(GLCore::Timestep ts)
{
	
}

void SliceViewer::OnImGuiRender()
{
	ImGui::Begin("Slice Viewer");

	

	float ratio_left = 0.8;
	// Child 
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * ratio_left, ImGui::GetContentRegionAvail().y), false, window_flags);


		ImGuiIO& io = ImGui::GetIO();
		ImTextureID my_tex_id = io.Fonts->TexID;
		float my_tex_w = (float)io.Fonts->TexWidth;
		float my_tex_h = (float)io.Fonts->TexHeight;
		float image_w, image_h;
		float frame_w, frame_h;
		frame_w = ImGui::GetContentRegionAvail().x * 0.6f;
		frame_h = ImGui::GetContentRegionAvail().y;

		float frame_ratio = frame_h / frame_w;
		float tex_ratio = my_tex_h / my_tex_w;
		if (frame_ratio>tex_ratio)
		{
			image_w = frame_w;
			image_h = tex_ratio * image_w;
		}
		else
		{
			image_h = my_tex_h;
			image_w = image_h / tex_ratio;
		}

		//std::cout << frame_w << " " << frame_h << " " << image_w << " "<<image_h<< std::endl;


		{
	
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
			ImGui::Image(my_tex_id, ImVec2(image_w, image_h), uv_min, uv_max, tint_col, border_col);
		}

		ImGui::EndChild();
	}

	ImGui::SameLine();

	// child 
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		//std::cout << ImGui::GetContentRegionAvail().y << std::endl;
		ImGui::BeginChild("ChildR", ImVec2(ImGui::GetContentRegionAvail().x * (1-ratio_left), ImGui::GetContentRegionAvail().y), false, window_flags);
		static int int_value = 0;
		ImGui::VSliderInt("##int", ImVec2(30, ImGui::GetContentRegionAvail().y), &int_value, 0, 1000);
		ImGui::SameLine();
		ImGui::EndChild();
		
	}
	

	ImGui::End();
}

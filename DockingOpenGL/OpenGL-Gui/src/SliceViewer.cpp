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
	ImGui::Begin(u8"切片设置");

	static bool  keep_aspect=true;
	static float expected_x_size;
	static float layer_thickness;
	
	ImVec2 slicer_setting_region_size = ImGui::GetContentRegionAvail();
	float region_width = slicer_setting_region_size.x-10;
	
	float widget_pos_x = 180;
	// Scale setting
	{
		
		using namespace ImGui;
		ImGui::PushItemWidth(30);
		Text(u8"保持比例缩放"); 
		SameLine(); 
		SetCursorPosX(widget_pos_x);
		PushID("##cb_keep_aspect");
		ImGui::Checkbox("", &keep_aspect);
		PopID();
	
		const char* expected_direction[] = { "X","Y","Z" };
		static int curr_direction = 0;
		static bool item_disabled = false;
		ImGui::PopItemWidth();
	
		
		ImGui::PushItemWidth(100);
		Text(u8"缩放方向"); 
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("combo_scale_direction");
		ImGui::Combo("", &curr_direction, expected_direction, IM_ARRAYSIZE(expected_direction), IM_ARRAYSIZE(expected_direction));
		PopID();
	
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(100);

		Text(u8"期望尺寸(μm)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("infloat_x_size");
		ImGui::InputFloat("", &expected_x_size);
		PopID();
		ImGui::PopItemWidth();
	}

	// Layer height
	{
		using namespace ImGui;
		const char* slice_strategy[] = { u8"等厚",u8"等弧度",u8"斜率自适应" };
		static int curr_strategy = 0;
		Text(u8"切片策略");
		SameLine();
		SetCursorPosX(widget_pos_x);

		ImGui::PushItemWidth(100);
		PushID("combo_slice_strategy");
		ImGui::Combo("", &curr_strategy, slice_strategy, IM_ARRAYSIZE(slice_strategy), IM_ARRAYSIZE(slice_strategy));
		PopID();
		ImGui::PopItemWidth();

		static float layer_thickness;
		ImGui::PushItemWidth(100);
		Text(u8"切片厚度(μm)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("infloat_thickness");
		ImGui::InputFloat("", &layer_thickness);
		PopID();
		ImGui::PopItemWidth();

		ButtonCenteredOnLine(u8"     切片     ");

	}


	{
		using namespace ImGui;
		NewLine(); Separator(); NewLine();

		static bool with_png = false;
		Text(u8"输出预览图(png)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("cb_with_png");
		ImGui::Checkbox("", &with_png);
		PopID();

		static bool roi_mode = false;
		Text(u8"设置Roi(μm)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("ROI Mode");
		ImGui::Checkbox("", &roi_mode);
		PopID();

		if (roi_mode)
		{
			static float roi_left, roi_right, roi_top, roi_bottom;
			PushItemWidth(60);

			SetCursorPosX(widget_pos_x);
			Text(u8"左:");
			SameLine();
			PushID("1");
			InputFloat("", &roi_left,0,0,"%.2f");
			PopID();

			SetCursorPosX(widget_pos_x);
			Text(u8"右:");
			SameLine();
			PushID("2");
			InputFloat("", &roi_right, 0, 0, "%.2f");
			PopID();

			SetCursorPosX(widget_pos_x);
			Text(u8"上:");
			SameLine();
			PushID("3");
			InputFloat("", &roi_top, 0, 0, "%.2f");
			PopID();

			SetCursorPosX(widget_pos_x);
			Text(u8"下:");
			SameLine();
			PushID("4");
			InputFloat("", &roi_bottom, 0, 0, "%.2f");
			PopID();
			PopItemWidth();
		}

		static int x_limit = 1000;
		static int y_limit = 100000;
		Text(u8"单元视场大小(pixels)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		ImGui::PushItemWidth(100);
		ImGui::InputInt(u8"宽", &x_limit,0); 
		SetCursorPosX(widget_pos_x);
		ImGui::InputInt(u8"高", &y_limit,0);
		ImGui::PopItemWidth();

		static float pixel_size=100;
		Text(u8"像素尺寸(nm)");
		SameLine();
		SetCursorPosX(widget_pos_x);
		ImGui::PushItemWidth(100);
		PushID("infloat_pixel_size");
		ImGui::InputFloat("", &pixel_size,0,0,"%.1f");
		PopID();
		PopItemWidth();

		static bool comp_mode = false;
		Text(u8"压缩转出数据");
		SameLine();
		SetCursorPosX(widget_pos_x);
		PushID("cb_comp_mode");
		ImGui::Checkbox(u8"", &comp_mode);
		PopID();

		ButtonCenteredOnLine(u8"     导出     ");
	}


	ImGui::End();





	ImGui::Begin(u8"切片预览");
	// Child 
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x -35, ImGui::GetContentRegionAvail().y), false, window_flags);

		ImGuiIO& io = ImGui::GetIO();
		ImTextureID my_tex_id = io.Fonts->TexID;
		float my_tex_w = (float)io.Fonts->TexWidth;
		float my_tex_h = (float)io.Fonts->TexHeight;
		float image_w, image_h;
		float frame_w, frame_h;
		frame_w = ImGui::GetContentRegionAvail().x-5;
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
		ImGui::BeginChild("ChildR", ImVec2(0, ImGui::GetContentRegionAvail().y), false, window_flags);
		static int int_value = 0;

		//std::cout << "ssd" << ImGui::GetContentRegionAvail().x;

		ImGui::VSliderInt("##int", ImVec2(21, ImGui::GetContentRegionAvail().y-3), &int_value, 0, 1000,"");

		std::string tip_layer_id = "layer: " + std::to_string(int_value);


		if (ImGui::IsItemActive() || ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", tip_layer_id.c_str());

		//ImGui::SameLine();
		ImGui::EndChild();
		
	}
	

	ImGui::End();
}

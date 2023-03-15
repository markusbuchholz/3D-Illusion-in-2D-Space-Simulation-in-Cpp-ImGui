// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <Eigen/Dense>

//---------------------------------------------------------------

bool SIMULATION = true;

int w = 1000;
int h = 1000;

int size = 1;
int rows = 1000;
int cols = 1000;

//---------------------------------------------------------------

int generateRandom()
{

	std::random_device engine;
	std::mt19937 generator(engine());
	std::uniform_int_distribution<int> distrib(1, 10);
	return distrib(generator);
}
//---------------------------------------------------------------

Eigen::Matrix<float, 3, 3> RotX(float ang)
{

	float c = std::cos((M_PI / 180) * ang);
	float s = std::sin((M_PI / 180) * ang);

	Eigen::Matrix<float, 3, 3> Rx;
	Rx << 1.0f, 0.0f, 0.0f,
		0.0f, c, -s,
		0.0f, s, c;
	return Rx;
}

//---------------------------------------------------------------
Eigen::Matrix<float, 3, 3> RotY(float ang)
{

	float c = std::cos((M_PI / 180) * ang);
	float s = std::sin((M_PI / 180) * ang);

	Eigen::Matrix<float, 3, 3> Ry;
	Ry << c, 0.0f, s,
		0.0f, 1.0f, 0.0f,
		-s, 0.0f, c;
	return Ry;
}

//---------------------------------------------------------------
Eigen::Matrix<float, 3, 3> RotZ(float ang)
{

	float c = std::cos((M_PI / 180) * ang);
	float s = std::sin((M_PI / 180) * ang);

	Eigen::Matrix<float, 3, 3> Rz;
	Rz << c, -s, 0.0f,
		s, c, 0.0f,
		0.0f, 0.0f, 0.0f;
	return Rz;
}

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	SIMULATION = true;

	std::string title = "3D Projection";
	initImgui(w, h, title);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 line_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 255.0f / 255.0, 1.00f);
	ImVec4 dot_color = ImVec4(255.0f / 255.0, 0.0f / 255.0, 0.0f / 255.0, 1.00f);

	bool flag = true;

	float dt = 0.1f;
	float ang_x = 0.0f;
	float ang_y = 0.0f;
	float ang_z = 0.0f;
	float d = 200.0f;
	float radius = 4.0f;

	Eigen::Matrix<float, 3, 1> cx;
	cx << 500, 500, 200;
	//-----------------------------------------
	Eigen::Matrix<float, 3, 1> p0;
	p0 << 400, 400, 100;

	Eigen::Matrix<float, 3, 1> p1;
	p1 << 600, 400, 100;

	Eigen::Matrix<float, 3, 1> p2;
	p2 << 600, 600, 100;

	Eigen::Matrix<float, 3, 1> p3;
	p3 << 400, 600, 100;

	Eigen::Matrix<float, 3, 1> p4;
	p4 << 400, 400, 300;

	Eigen::Matrix<float, 3, 1> p5;
	p5 << 600, 400, 300;

	Eigen::Matrix<float, 3, 1> p6;
	p6 << 600, 600, 300;

	Eigen::Matrix<float, 3, 1> p7;
	p7 << 400, 600, 300;
	//-----------------------------------------

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		static int ang_slider_x = 0;
		static int ang_slider_y = 0;
		static int ang_slider_z = 0;

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		if (SIMULATION == false)
		{

			ang_x = (float)ang_slider_x;
			ang_y = (float)ang_slider_y;
			ang_z = (float)ang_slider_z;
		}
		else
		{
			ang_x += dt;
			ang_y += dt;
			ang_z += dt;
		}

		std::vector<Eigen::Matrix<float, 3, 1>> dots = {p0, p1, p2, p3, p4, p5, p6, p7};
		std::vector<Eigen::Matrix<float, 3, 1>> vertex;

		for (auto &p : dots)
		{
			Eigen::Matrix<float, 3, 1> px;
			Eigen::Matrix<float, 3, 1> py;
			Eigen::Matrix<float, 3, 1> pz;

			py = RotY(ang_y) * (p - cx) + cx;
			px = RotX(ang_x) * (py - cx) + cx;
			pz = RotZ(ang_z) * (px - cx) + cx;

			vertex.push_back((px, py, pz));
		}

		for (auto &v : vertex)
		{

			draw_list->AddCircleFilled({v(0, 0), v(1, 0)}, radius, ImColor(line_color));
		}

		std::vector<Eigen::Matrix<float, 3, 1>> front = {vertex[0], vertex[1], vertex[2], vertex[3], vertex[0]};

		for (int ii = 0; ii < front.size() - 1; ii++)
		{
			draw_list->AddLine({front[ii](0, 0), front[ii](1, 0)}, {front[ii + 1](0, 0), front[ii + 1](1, 0)}, ImColor(line_color), 2.0f);
		}

		std::vector<Eigen::Matrix<float, 3, 1>> back = {vertex[4], vertex[5], vertex[6], vertex[7], vertex[4]};

		for (int ii = 0; ii < back.size() - 1; ii++)
		{
			draw_list->AddLine({back[ii](0, 0), back[ii](1, 0)}, {back[ii + 1](0, 0), back[ii + 1](1, 0)}, ImColor(line_color), 2.0f);
		}

		for (int ii = 0; ii < 4; ii++)
		{
			draw_list->AddLine({front[ii](0, 0), front[ii](1, 0)}, {back[ii](0, 0), back[ii](1, 0)}, ImColor(line_color), 2.0f);
		}

		draw_list->AddCircleFilled({cx(0, 0), cx(1, 0)}, radius, ImColor(dot_color));

		ImGui::PushItemWidth(300);
		ImGui::SliderInt("angX", &ang_slider_x, 0, 360);
		ImGui::PushItemWidth(300);
		ImGui::SliderInt("angY", &ang_slider_y, 0, 360);
		ImGui::PushItemWidth(300);
		ImGui::SliderInt("angZ", &ang_slider_z, 0, 360);

		ImGui::PopItemWidth();
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}

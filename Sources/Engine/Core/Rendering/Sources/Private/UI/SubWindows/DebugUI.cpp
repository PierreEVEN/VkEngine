#include "UI/SubWindows/DebugUI.h"
#include "Scene/Scene.h"
#include "Scene/SceneComponents/Camera.h"

void Rendering::DebugUI::DrawContent(const size_t& imageIndex)
{
	float avgFps = 0;

	fpsHistory[fpsFrameIndex] = 1 / (float)parent->GetDeltaTime();

	for (int i = 199; i >= 0; --i)
	{
		avgFps += fpsHistory[(fpsFrameIndex - i + 1000) % 1000];
	}
	avgFps /= 200;
	ImGui::Text(String(String("Framerate : ") + ToString(1 / parent->GetDeltaTime()) + " fps").GetData());
	if (G_ENABLE_MULTISAMPLING.GetValue())
	{
		ImGui::Text("MSAA samples : %d", G_MSAA_SAMPLE_COUNT);
	}
	else
	{
		ImGui::Text("MSAA samples : disabled");
	}
	if ((1 / parent->GetDeltaTime() > maxFpsHistory)) maxFpsHistory = 1 / (float(parent->GetDeltaTime()));
	TConVarUI<int32_t>::DrawUI(G_MAX_FRAMERATE);
	fpsFrameIndex = (fpsFrameIndex + 1) % 1000;
	ImGui::PlotLines("framerate", fpsHistory, IM_ARRAYSIZE(fpsHistory), fpsFrameIndex, String(String("average : ") + ToString(avgFps) + String(" / max : ") + ToString(maxFpsHistory)).GetData(), 0, maxFpsHistory, ImVec2(0, 80.0f));

	TConVarUI<bool>::DrawUI(G_ENABLE_MULTISAMPLING);
	TConVarUI<bool>::DrawUI(G_SLEEP_HIDLE_THREADS);
	TConVarUI<bool>::DrawUI(G_FULSCREEN_MODE);
	TConVarUI<bool>::DrawUI(G_ENABLE_VALIDATION_LAYERS);
	ImGui::Separator();
	ImGui::Text("camera transform");
	ImGui::SliderFloat3("camera position", cameraPosition.coords, -3, 3);
	ImGui::SliderFloat3("camera rotation", cameraRotation.coords, -180, 180);
	ImGui::SliderFloat("fov", &fov, 1, 179);
	ImGui::SliderFloat("clip min", &mi, 0, 1);
	ImGui::SliderFloat("clip max", &mx, 0, 1000);
	ImGui::Separator();
	ImGui::Text("Default texture");
	ImGui::Image(G_DEFAULT_TEXTURE->GetTextureID(imageIndex), ImVec2(512, 512));


	if (ImGui::Button("log")) LOG("log");
	if (ImGui::Button("warning")) LOG_WARNING("warning");
	if (ImGui::Button("error")) LOG_ERROR("error");

	parent->GetCamera()->SetLocation(cameraPosition);
	parent->GetCamera()->SetRotation(cameraRotation);
	parent->GetCamera()->fieldOfView = fov;
	parent->GetCamera()->nearClipPlane = mi;
	parent->GetCamera()->farClipPlane = mx;
}
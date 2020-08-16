#include "UI/SubWindows/SubWindow.h"


Rendering::SubWindow::SubWindow(const String& name, SubWindow* parentWindow /*= nullptr*/, bool bInRenderInParent)
	: windowName(name), parent(parentWindow), bKeepOpen(true), bRenderInParent(bInRenderInParent && parentWindow)
{
	myWindowId = currentWindowID;
	currentWindowID++;
	registeredWindows.push_back(this);
	if (parent) {
		parent->childs.push_back(this);
	}
}

void Rendering::SubWindow::ProcessSubWindows(const size_t& imageIndex)
{
	for (int64_t i = registeredWindows.size() - 1; i >= 0; --i) {
		if (!registeredWindows[i]->bKeepOpen) {
			delete registeredWindows[i];
			registeredWindows.erase(registeredWindows.begin() + i);
		}
		else {
			if (!registeredWindows[i]->parent) {
				registeredWindows[i]->Draw(imageIndex);
			}
		}
	}
}

Rendering::SubWindow::~SubWindow()
{
	if (parent) {
		for (int64_t i = parent->childs.size() - 1; i >= 0; --i) {
			if (parent->childs[i] == this) {
				parent->childs.erase(parent->childs.begin() + i);
			}
		}
	}
	for (int64_t i = childs.size() - 1; i >= 0; --i)
	{
		childs[i]->bKeepOpen = false;
		childs[i]->parent = nullptr;
	}
	childs.clear();
}

void Rendering::SubWindow::Draw(const size_t& imageIndex)
{
	for (auto& child : childs) {
		if (!child->bRenderInParent) {
			child->Draw(imageIndex);
		}
	}

	if (bRenderInParent && parent) {
		if (ImGui::BeginChild((windowName + "##" + ToString(myWindowId)).GetData(), ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize(windowName.GetData()).x / 2.f, 0));
				ImGui::Text(windowName.GetData());
				ImGui::EndMenuBar();
			}
			for (auto& child : childs) {
				if (child->bRenderInParent) {
					child->Draw(imageIndex);
				}
			}

			DrawContent(imageIndex);
			ImGui::End();
		}
	}
	else
	{
		if (ImGui::Begin((windowName + "##" + ToString(myWindowId)).GetData(), &bKeepOpen))
		{
			DrawContent(imageIndex);
			for (auto& child : childs) {
				if (child->bRenderInParent) {
					child->Draw(imageIndex);
				}
			}

			ImGui::End();
		}
	}
}


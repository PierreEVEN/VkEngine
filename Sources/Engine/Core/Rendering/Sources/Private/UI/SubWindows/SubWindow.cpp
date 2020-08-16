#include "UI/SubWindows/SubWindow.h"

#include <stb_image.h>

void Rendering::SubWindow::LoadUIRessources()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels;

	pixels = stbi_load("Ressources/Textures/Icons/icon-directory.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	directoryIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-file.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	fileIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-upArrowCircle.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	upArrowCircleIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);
}

void Rendering::SubWindow::DestroyUIRessources()
{
	delete directoryIcon;
	delete fileIcon;
	delete upArrowCircleIcon;
}


Rendering::SubWindow::SubWindow(const String& name, SubWindow* parentWindow /*= nullptr*/)
	: windowName(name), parent(parentWindow), bKeepOpen(true)
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
			registeredWindows[i]->Draw(imageIndex);
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
	if (ImGui::Begin((windowName + "##" + ToString(myWindowId)).GetData(), &bKeepOpen))
	{
		DrawContent(imageIndex);
		ImGui::End();
	}
}


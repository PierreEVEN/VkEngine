#include "UI/SubWindows/TextureImporterWindow.h"
#include "UI/SubWindows/FileExplorer.h"
#include "Assets/AssetFactory.h"
#include "Assets/Texture2D.h"

Rendering::TextureImportWindow::TextureImportWindow()
	: SubWindow("Import texture")
{
	FileExplorer* fe = new FileExplorer(".", "Selecte texture", this, TAssetFactory<Texture2D>::GetDesiredImportFormats(), true);
	fe->OnApplyPath.Add(this, &TextureImportWindow::OnPickedTexture);
	fe->OnCancelExplorer.Add((SubWindow*)this, &SubWindow::RequestClose);
}

void Rendering::TextureImportWindow::OnPickedTexture(const String& path)
{
	TAssetFactory<Texture2D>::MakeTransient(TAssetFactory<Texture2D>::ImportFromPath(path));
	RequestClose();
}


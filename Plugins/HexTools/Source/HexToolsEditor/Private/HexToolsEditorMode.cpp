// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "HexToolsEditorMode.h"
#include "HexToolsEditorToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FHexToolsEditorMode::EM_HexToolsEditorModeId = TEXT("EM_HexToolsEditorMode");

FHexToolsEditorMode::FHexToolsEditorMode()
{

}

FHexToolsEditorMode::~FHexToolsEditorMode()
{

}

void FHexToolsEditorMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FHexToolsEditorToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FHexToolsEditorMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FHexToolsEditorMode::UsesToolkits() const
{
	return true;
}





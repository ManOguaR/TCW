// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "HexToolsEditor.h"
#include "HexToolsEditorMode.h"

#define LOCTEXT_NAMESPACE "FEdModeModule"

void FEdModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FHexToolsEditorMode>(FHexToolsEditorMode::EM_HexToolsEditorModeId, LOCTEXT("HexToolsEditorModeName", "HexToolsEditorMode"), FSlateIcon(), true);
}

void FEdModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FHexToolsEditorMode::EM_HexToolsEditorModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEdModeModule, HexToolsEditor)
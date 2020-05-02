// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "HexTools.h"

#define LOCTEXT_NAMESPACE "FHexToolsModule"

DEFINE_LOG_CATEGORY(HexToolsLog);
DEFINE_LOG_CATEGORY(HexToolsLogErrors);

void FHexToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FHexToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHexToolsModule, HexTools)
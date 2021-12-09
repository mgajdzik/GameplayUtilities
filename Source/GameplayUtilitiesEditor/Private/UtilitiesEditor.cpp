// Copyright Mateusz Gajdzik. All Rights Reserved.

#include "GameplayUtilitiesEditor/Public/GameplayUtilitiesEditor.h"

#define LOCTEXT_NAMESPACE "FGameplayUtilitiesEditorModule"

void FGameplayUtilitiesEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGameplayUtilitiesEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameplayUtilitiesEditorModule, GameplayUtilitiesEditor)
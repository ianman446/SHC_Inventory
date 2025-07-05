// Copyright Epic Games, Inc. All Rights Reserved.

#include "SHC_Inventory.h"

#define LOCTEXT_NAMESPACE "FSHC_InventoryModule"

DEFINE_LOG_CATEGORY(LogInventory);

void FSHC_InventoryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSHC_InventoryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSHC_InventoryModule, SHC_Inventory)
// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"


void UInv_SpatialInventory::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Button_All->OnClicked.AddDynamic(this, &ThisClass::ShowAllItems);
    Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
    Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
    Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);
    Button_Quest->OnClicked.AddDynamic(this, &ThisClass::ShowQuestItems);
    Button_ItemCategory01->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory01);
    Button_ItemCategory02->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory02);
    Button_ItemCategory03->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory03);

    ShowAllItems();
}

void UInv_SpatialInventory::ShowAllItems()
{
    SetActiveGrid(Grid_All, Button_All);
}

void UInv_SpatialInventory::ShowEquippables()
{
    SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
    SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
    SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventory::ShowQuestItems()
{
    SetActiveGrid(Grid_Quest, Button_Quest);
}

void UInv_SpatialInventory::ShowItemCategory01()
{
    SetActiveGrid(Grid_ItemCategory01, Button_ItemCategory01);
}

void UInv_SpatialInventory::ShowItemCategory02()
{
    SetActiveGrid(Grid_ItemCategory02, Button_ItemCategory02);
}

void UInv_SpatialInventory::ShowItemCategory03()
{
    SetActiveGrid(Grid_ItemCategory03, Button_ItemCategory03);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
    DisableButton(Button);

    Switcher->SetActiveWidget(Grid);
}

void UInv_SpatialInventory::DisableButton(UButton* Button)
{
    Button_All->SetIsEnabled(true);
    Button_Equippables->SetIsEnabled(true);
    Button_Consumables->SetIsEnabled(true);
    Button_Craftables->SetIsEnabled(true);
    Button_Quest->SetIsEnabled(true);
    Button_ItemCategory01->SetIsEnabled(true);
    Button_ItemCategory02->SetIsEnabled(true);
    Button_ItemCategory03->SetIsEnabled(true);
    Button->SetIsEnabled(false);
}

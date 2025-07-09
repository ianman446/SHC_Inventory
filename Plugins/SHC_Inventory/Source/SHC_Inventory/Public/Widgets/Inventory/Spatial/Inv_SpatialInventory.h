// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UInv_InventoryGrid;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class SHC_INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_All;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Quest;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_ItemCategory01;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_ItemCategory02;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_ItemCategory03;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_All;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Quest;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ItemCategory01;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ItemCategory02;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ItemCategory03;

	UFUNCTION()
	void ShowAllItems();

	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void ShowQuestItems();

	UFUNCTION()
	void ShowItemCategory01();

	UFUNCTION()
	void ShowItemCategory02();

	UFUNCTION()
	void ShowItemCategory03();

	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);
	void DisableButton(UButton* Button);
};

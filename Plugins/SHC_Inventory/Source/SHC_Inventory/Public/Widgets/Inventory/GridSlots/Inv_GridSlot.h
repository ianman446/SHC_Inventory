// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UImage;
class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut,
	// TODO: Might need to expand the state to cover for tranfering items around like a valid placement or whatever.
};

UCLASS()
class SHC_INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	EInv_GridSlotState GetGridSlotState() { return GridSlotState; }

	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInv_InventoryItem* InvItem);

	void SetTileIndex(int32 Index) { TileIndex = Index; }
	int32 GetTileIndext() const { return TileIndex; }

	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }

	bool IsAvailable() const { return bAvailable; }
	void SetIsAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }

	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

private:

	int32 TileIndex;
	int32 StackCount;
	int32 UpperLeftIndex = INDEX_NONE;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bAvailable = true;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState = EInv_GridSlotState::Unoccupied;


};

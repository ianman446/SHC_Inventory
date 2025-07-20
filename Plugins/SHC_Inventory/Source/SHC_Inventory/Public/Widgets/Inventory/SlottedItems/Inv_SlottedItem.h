// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItem.generated.h"

/**
 * 
 */

class UImage;
class UInv_InventoryItem;
class UTextBlock;

UCLASS()
class SHC_INVENTORY_API UInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UImage* GetImageIcon() const { return Image_Icon; }

	int32 GetGridIndex() const { return GridIndex; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }

	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }

	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	void SetInventoryItem(UInv_InventoryItem* Item);

	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }

	void SetImageBrush(const FSlateBrush& Brush) const;

	void UpdateStackCount(int32 StackCount);

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 GridIndex = 0;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable = false;
};

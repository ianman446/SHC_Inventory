// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Inv_EquippedSlottedItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedSlottedItemClicked, class UInv_EquippedSlottedItem*, SlottedItem);

UCLASS()
class SHC_INVENTORY_API UInv_EquippedSlottedItem : public UInv_SlottedItem
{
	GENERATED_BODY()

public:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

    void SetEquipmentTypeTag(const FGameplayTag& Tag) { EquipmentTypeTag = Tag; }
    FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }

    FOnEquippedSlottedItemClicked OnEquippedSlottedItemClicked;

private:

	UPROPERTY()
    FGameplayTag EquipmentTypeTag;
	
};

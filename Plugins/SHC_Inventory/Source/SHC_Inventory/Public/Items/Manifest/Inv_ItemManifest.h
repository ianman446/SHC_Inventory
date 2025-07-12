// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemManifest.generated.h"

/**
 * The Item Manifest contains all of the necessary data
 * for creating a new Inventory Item
 */

class UInv_InventoryItem;
struct FInv_ItemFragment;

USTRUCT(BlueprintType)
struct SHC_INVENTORY_API FInv_ItemManifest
{
    GENERATED_BODY()

    UInv_InventoryItem* Manifest(UObject* NewOuter);
    EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
    FGameplayTag GetItemType() { return ItemType; }

private:

    UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct), Category = "Inventory")
    TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    EInv_ItemCategory ItemCategory = EInv_ItemCategory::None;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FGameplayTag ItemType;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_InventoryComponent;
class UInv_ItemComponent;
class APlayerController;
/**
 * 
 */
UCLASS()
class SHC_INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComponent(UInv_ItemComponent* ItemComp);
};

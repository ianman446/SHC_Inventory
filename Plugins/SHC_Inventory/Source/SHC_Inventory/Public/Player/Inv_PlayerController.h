// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;
class UInv_InventoryComponent;

UCLASS()
class SHC_INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    AInv_PlayerController();
    virtual void Tick(float DeltaTime) override;

    /// <summary>
    /// 
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ToggleInventory();

protected:
	virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    /// <summary>
    /// This is the function called to start an interaction like picking up an item
    /// </summary>
    void PrimaryInteract();
    void CreateHUDWidget();

    /// <summary>
    /// This function is called every frame on tick to see if the player is looking at an interactable actor like an item
    /// </summary>
    void TraceForItem();
    
    TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

    /// <summary>
    /// Default Inventory Input Mapping Context for interaction and Hot Slots
    /// </summary>
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TObjectPtr<UInputMappingContext> DefaultIMC;

    /// <summary>
    /// The specific Input Action for Interacting with a world actor like an item
    /// </summary>
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TObjectPtr<UInputAction> PrimaryInteractAction;

    /// <summary>
    /// The specific Input Action for Opening or Closing the inventory widget
    /// </summary>
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TObjectPtr<UInputAction> ToggleInventoryAction;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

    UPROPERTY()
    TObjectPtr<UInv_HUDWidget> HUDWidget;

    /// <summary>
    /// How long we want to trace from the center of the player's viewport for Interactable Actors like Items
    /// </summary>
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    double TraceLength;
    
    /// <summary>
    /// DO NOT FORGET TO CREATE AND ASSIGN ITEM COLLISION AND PRESET
    /// </summary>
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TEnumAsByte<ECollisionChannel> ItemTraceChannel;

    TWeakObjectPtr<AActor> ThisActor;
    TWeakObjectPtr<AActor> LastActor;
};

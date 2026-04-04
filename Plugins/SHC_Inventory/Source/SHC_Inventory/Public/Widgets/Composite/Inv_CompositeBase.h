// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

#include "Inv_CompositeBase.generated.h"

/**
 * 
 */

UCLASS()
class SHC_INVENTORY_API UInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
    FGameplayTag GetFragmentTag() const { return FragmentTag; }
    void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }
	virtual void Collapse();
	virtual void Expand();

    using FuncType = TFunction<void(UInv_CompositeBase*)>; 

	// a function to call another function that takes a UInv_CompositeBase as parameter, used to apply a function to all the children of the composite
    virtual void ApplyFunction(FuncType Function) {} 

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag;
};

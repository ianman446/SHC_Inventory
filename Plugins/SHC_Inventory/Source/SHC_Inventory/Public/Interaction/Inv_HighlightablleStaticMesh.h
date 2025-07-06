// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interaction/Inv_HighlightableInterface.h"
#include "Inv_HighlightablleStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class SHC_INVENTORY_API UInv_HighlightablleStaticMesh : public UStaticMeshComponent, public IInv_HighlightableInterface
{
	GENERATED_BODY()
public:
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};

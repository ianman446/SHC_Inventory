// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_ProxyMesh.generated.h"

class UInv_EquipmentComponent;

UCLASS(Blueprintable)
class SHC_INVENTORY_API AInv_ProxyMesh : public AActor
{
	GENERATED_BODY()
	
public:	

	AInv_ProxyMesh();
    USkeletalMeshComponent* GetMesh() const { return Mesh; }

protected:

	virtual void BeginPlay() override;

private:	

	// This is the mesh of the player-controled Character.
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TObjectPtr<UInv_EquipmentComponent> EquipmentComponent;

	// This is the proxy mesh we will see in the inventory menu.
	UPROPERTY(VisibleAnywhere)
    TObjectPtr<USkeletalMeshComponent> Mesh;

	FTimerHandle TimerForNextTick;

	void DelayedInitializeOwner();
    // Hey, on next tick, call this function if we haven't initialized the proxy mesh yet.
    void DelayedInitialization();
};

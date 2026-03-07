// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter)
{
    UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
    Item->SetItemManifest(*this);

    return Item;
}

void FInv_ItemManifest::SpawnPickUpActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!IsValid(PickUpActorClass)||!IsValid(WorldContextObject)) return;

    AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickUpActorClass, SpawnLocation, SpawnRotation);
    if (!IsValid(SpawnedActor)) return;

    // Set the item manifest; item category, item type, etc.
    UInv_ItemComponent* ItemComponent = SpawnedActor->FindComponentByClass<UInv_ItemComponent>();
    check(ItemComponent);

    ItemComponent->InitItemManifest(*this);
}

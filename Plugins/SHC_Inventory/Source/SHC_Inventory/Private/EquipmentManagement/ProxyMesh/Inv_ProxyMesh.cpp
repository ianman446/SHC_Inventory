// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/ProxyMesh/Inv_ProxyMesh.h"

#include "EquipmentManagement/Components/Inv_EquipmentComponent.h"
#include "GameFramework/Character.h"

AInv_ProxyMesh::AInv_ProxyMesh()
{

	PrimaryActorTick.bCanEverTick = false;
    bReplicates = false;
    SetReplicates(false);

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
    Mesh->SetupAttachment(RootComponent);

    EquipmentComponent = CreateDefaultSubobject<UInv_EquipmentComponent>("Equipment");
    EquipmentComponent->SetOwningSkeletalMesh(Mesh);
    EquipmentComponent->SetIsProxy(true);
}


void AInv_ProxyMesh::BeginPlay()
{
	Super::BeginPlay();
    DelayedInitializeOwner();
}

void AInv_ProxyMesh::DelayedInitializeOwner()
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        UE_LOG(LogTemp, Warning, TEXT("AInv_ProxyMesh::DelayedInitializeOwner - No world found for proxy mesh %s. Will try again on next tick."), *GetName());
        DelayedInitialization();
        return;
    }

    APlayerController* PC = World->GetFirstPlayerController();
    if (!IsValid(PC))
    {
        UE_LOG(LogTemp, Warning, TEXT("AInv_ProxyMesh::DelayedInitializeOwner - No player controller found for proxy mesh %s. Will try again on next tick."), *GetName());
        DelayedInitialization();
        return;
    }

    ACharacter* Character = Cast<ACharacter>(PC->GetPawn());
    if (!IsValid(Character))
    {
        UE_LOG(LogTemp, Warning, TEXT("AInv_ProxyMesh::DelayedInitializeOwner - No character found for proxy mesh %s. Will try again on next tick."), *GetName());
        DelayedInitialization();
        return;
    }

    USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
    if (!IsValid(CharacterMesh))
    {
        UE_LOG(LogTemp, Warning, TEXT("AInv_ProxyMesh::DelayedInitializeOwner - No source mesh found for proxy mesh %s. Will try again on next tick."), *GetName());
        DelayedInitialization();
        return;
    }

    SourceMesh = CharacterMesh;
    Mesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
    Mesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());

    EquipmentComponent->InitializeOwner(PC);
}

void AInv_ProxyMesh::DelayedInitialization()
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &ThisClass::DelayedInitializeOwner);
    GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}



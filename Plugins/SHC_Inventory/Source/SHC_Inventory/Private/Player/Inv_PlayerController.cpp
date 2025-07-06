// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h" 
#include "Items/Components/Inv_ItemComponent.h"
#include "Interaction/Inv_HighlightableInterface.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


AInv_PlayerController::AInv_PlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    TraceLength = 1000.0;
    ItemTraceChannel = ECC_GameTraceChannel1;
}

void AInv_PlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TraceForItem();
}

void AInv_PlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (IsValid(Subsystem))
    {
        Subsystem->AddMappingContext(DefaultIMC, 0);
    }

    InventoryComponent = FindComponentByClass< UInv_InventoryComponent>();

    CreateHUDWidget();

}

void AInv_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
    EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AInv_PlayerController::ToggleInventory);
}

void AInv_PlayerController::PrimaryInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Interaction action"));
}

void AInv_PlayerController::ToggleInventory()
{
    if (!InventoryComponent.IsValid()) return;

    InventoryComponent->ToggleInventory();
}

void AInv_PlayerController::CreateHUDWidget()
{
    if (!IsLocalController()) return;

    HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
    if (IsValid(HUDWidget))
    {
        HUDWidget->AddToViewport();
    }
}

void AInv_PlayerController::TraceForItem()
{
    if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
    FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);
    const FVector2D ViewportCenter = ViewportSize / 2.f;
    FVector TraceStart;
    FVector Forward;
    if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;

    const FVector TraceEnd = TraceStart + Forward * TraceLength;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

    if (!ThisActor.IsValid())
    {
        if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
    }

    LastActor = ThisActor;
    ThisActor = HitResult.GetActor();

    if (ThisActor == LastActor) return;

    if (ThisActor.IsValid())
    {
        if (UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_HighlightableInterface::StaticClass()); IsValid(Highlightable))
        {
            IInv_HighlightableInterface::Execute_Highlight(Highlightable);
        }

        UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass< UInv_ItemComponent>();
        if (!IsValid(ItemComponent)) return;

        if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
    }

    if (LastActor.IsValid())
    {
        
        if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_HighlightableInterface::StaticClass()); IsValid(Highlightable))
        {
            IInv_HighlightableInterface::Execute_UnHighlight(Highlightable);
        }
    }

}

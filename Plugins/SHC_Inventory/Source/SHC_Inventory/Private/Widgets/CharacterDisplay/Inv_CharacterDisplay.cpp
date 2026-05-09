// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterDisplay/Inv_CharacterDisplay.h"

#include "Kismet/GameplayStatics.h"
#include "EquipmentManagement/ProxyMesh/Inv_ProxyMesh.h"
#include "Blueprint/WidgetLayoutLibrary.h"


FReply UInv_CharacterDisplay::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
    CurrentPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
    LastPosition = CurrentPosition;

    bIsDragging = true;
    return FReply::Handled();
}

FReply UInv_CharacterDisplay::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    bIsDragging = false;
    return FReply::Handled();
}

void UInv_CharacterDisplay::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
    Super::NativeOnMouseLeave(MouseEvent);
    bIsDragging = false;
}

void UInv_CharacterDisplay::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(this, AInv_ProxyMesh::StaticClass(), FoundActors);

    if (!FoundActors.IsValidIndex(0)) return;

    AInv_ProxyMesh* ProxyMesh = Cast<AInv_ProxyMesh>(FoundActors[0]);
    if (!IsValid(ProxyMesh)) return;

    Mesh = ProxyMesh->GetMesh();
}

void UInv_CharacterDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!bIsDragging) return;

    LastPosition = CurrentPosition;
    CurrentPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

    const float HorizontalDelta = LastPosition.X - CurrentPosition.X;

    if (!Mesh.IsValid()) return;
    Mesh->AddRelativeRotation(FRotator(0.f, HorizontalDelta , 0.f));
}

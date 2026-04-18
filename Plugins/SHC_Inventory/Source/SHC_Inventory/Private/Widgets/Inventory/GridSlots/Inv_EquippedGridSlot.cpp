// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Overlay.h" 
#include "Components/OverlaySlot.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (!IsAvailable()) return;
    UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoverItemWidget(GetOwningPlayer());
    if (!IsValid(HoverItem)) return;

    if (HoverItem->GetItemTag().MatchesTag(EquimentTypeTag))
    {
        SetOccupiedTexture();
        Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
    if (!IsAvailable()) return;
    UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoverItemWidget(GetOwningPlayer());
    if (!IsValid(HoverItem)) return;

    if (HoverItem->GetItemTag().MatchesTag(EquimentTypeTag))
    {
        SetUnoccupiedTexture();
        Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
    }
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    EquippedGridSlotClicked.Broadcast(this, EquimentTypeTag);
    return FReply::Handled();
}

UInv_EquippedSlottedItem* UInv_EquippedGridSlot::OnItemEquipped(UInv_InventoryItem* item, const FGameplayTag& EquipmentTag, float TileSize)
{
    // Check the equipment type tag
    if (!EquipmentTag.MatchesTagExact(EquimentTypeTag)) return nullptr;

    // get grid dimensions
    const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(item, FragmentTags::GridFragment);
    if (!GridFragment) return nullptr;
    const FIntPoint GridDimensions = GridFragment->GetGridSize();
    
    // calculate the draw size for the equipped slotted item
    const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
    const FVector2D DrawSize = GridDimensions * IconTileWidth;

    // create the equipped slotted items widget
    EquippedSlottedItem = CreateWidget<UInv_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);
    
    // set the slotted items inventory item
    EquippedSlottedItem->SetInventoryItem(item);

    // set the slotted items equipment type tag
    EquippedSlottedItem->SetEquipmentTypeTag(EquimentTypeTag);

    // hide the stack count widget on the slotted item
    EquippedSlottedItem->UpdateStackCount(0);

    // set inventory item on this class
    SetInventoryItem(item);

    // set the image brush on the equipped slotted item
    const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(item, FragmentTags::IconFragment);
    if (!ImageFragment) return nullptr;

    FSlateBrush Brush;
    Brush.SetResourceObject(ImageFragment->GetIcon());
    Brush.DrawAs = ESlateBrushDrawType::Image;
    Brush.ImageSize = DrawSize;

    EquippedSlottedItem->SetImageBrush(Brush);

    // add the slotted item as a child to this widget's overlay
    Overlay_Root->AddChildToOverlay(EquippedSlottedItem);
    FGeometry OverlayGeometry = Overlay_Root->GetCachedGeometry();
    FVector2f OverlayPos = OverlayGeometry.Position;
    FVector2f OverlaySize = OverlayGeometry.GetLocalSize();

    const float LeftPadding = OverlaySize.X / 2.f - DrawSize.X / 2.f;
    const float TopPadding = OverlaySize.Y / 2.f - DrawSize.Y / 2.f;

    UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
    OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));

    // return the equipped slotted item widget    
    return EquippedSlottedItem;
}

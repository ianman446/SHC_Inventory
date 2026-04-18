// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "SHC_Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"



void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_All->OnClicked.AddDynamic(this, &ThisClass::ShowAllItems);
	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);
	Button_Quest->OnClicked.AddDynamic(this, &ThisClass::ShowQuestItems);
	Button_ItemCategory01->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory01);
	Button_ItemCategory02->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory02);
	Button_ItemCategory03->OnClicked.AddDynamic(this, &ThisClass::ShowItemCategory03);

    Grid_Equippables->SetOwningCanvasPanel(CanvasPanel);
    Grid_Consumables->SetOwningCanvasPanel(CanvasPanel);
    Grid_Craftables->SetOwningCanvasPanel(CanvasPanel);
    Grid_Quest->SetOwningCanvasPanel(CanvasPanel);
    Grid_ItemCategory01->SetOwningCanvasPanel(CanvasPanel);
    Grid_ItemCategory02->SetOwningCanvasPanel(CanvasPanel);
    Grid_ItemCategory03->SetOwningCanvasPanel(CanvasPanel);
    Grid_All->SetOwningCanvasPanel(CanvasPanel);

	ShowAllItems();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
        UInv_EquippedGridSlot* EquippedGridSlot = Cast<UInv_EquippedGridSlot>(Widget);
		if (IsValid(EquippedGridSlot))
		{
            EquippedGridSlots.Add(EquippedGridSlot);
            EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
        }
    });
}

void UInv_SpatialInventory::EquippedGridSlotClicked(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag)
{
	// check to see if we can equip the hover item
    if (!CanEquipHoveredItem(EquippedGridSlot, EquipmentTypeTag)) return;

    UInv_HoverItem* HoverItem = GetHoverItem();

    // create an equipped slotted item and add it to the Equipped Grid Slot (call EquippedGridSlot->OnItemEquipped())
    const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
    UInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag, 
		TileSize);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	// clear the hover item
    Grid_Equippables->ClearHoverItem();

	// inform the server that we've equipped an item (potentially unequip an item as well)
    UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	//RPC
	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	if(GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
    }
}

void UInv_SpatialInventory::EquippedSlottedItemClicked(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
	// remove the item description
	UInv_InventoryStatics::ItemUnHovered(GetOwningPlayer());
	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;

	// get item to equip
    UInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;

    // get item to unequip (if any)	
    UInv_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();

	// get the equipped grid slot holding this item
    UInv_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);
	
    // clear the slot of this item (set its iventory item to null and remove the slotted item from the slot)
    ClearSlotOfItem(EquippedGridSlot);

	// remove of the equipped slotted item from the quipped grid slot
    RemoveEquippedSlottedItem(EquippedSlottedItem);

	// assign previously erquipped item as the hover item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);

	// make a new equipped slotted item
    MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);

    // broadcast delegates for OnItemEquipped/OnItemUnequipped
    BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
}

FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActiveGrid->DropItem();
    return FReply::Handled();
}

void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    if (!IsValid(ItemDescription)) return;
    SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);

}

void UInv_SpatialInventory::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
		UInv_WidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

bool UInv_SpatialInventory::CanEquipHoveredItem(UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	UInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoveredItem() && IsValid(HeldItem) && !HoverItem->IsStackable() && HeldItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable && HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}

UInv_EquippedGridSlot* UInv_SpatialInventory::FindSlotWithEquippedItem(UInv_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const UInv_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
    });
    return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UInv_SpatialInventory::ClearSlotOfItem(UInv_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
        EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);        
    }
}

void UInv_SpatialInventory::RemoveEquippedSlottedItem(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;
	
	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
    }
	EquippedSlottedItem->RemoveFromParent();
}

void UInv_SpatialInventory::MakeEquippedSlottedItem(UInv_EquippedSlottedItem* EquippedSlottedItem, UInv_EquippedGridSlot* EquippedGridSlot, UInv_InventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	UInv_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(ItemToEquip,
		EquippedSlottedItem->GetEquipmentTypeTag(),
		UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize());

	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

    EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UInv_SpatialInventory::BroadcastSlotClickedDelegates(UInv_InventoryItem* ItemToEquip, UInv_InventoryItem* ItemToUnequip) const
{
    UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
    check(IsValid(InventoryComponent));
    InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if(GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
        InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
        InventoryComponent->OnItemUnequipped.Broadcast(ItemToUnequip);
    }
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComponent(ItemComponent))
	{
	case EInv_ItemCategory::All:
		return Grid_All->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Equippable:
		return Grid_Equippables->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Consumable:
		return Grid_Consumables->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Craftable:
		return Grid_Craftables->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::Quest:
		return Grid_Quest->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::ItemCategory01:
		return Grid_ItemCategory01->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::ItemCategory02:
		return Grid_ItemCategory02->HasRoomForItem(ItemComponent);
	case EInv_ItemCategory::ItemCategory03:
		return Grid_ItemCategory03->HasRoomForItem(ItemComponent);
	default:
		UE_LOG(LogInventory, Error, TEXT("UInv_SpatialInventory::HasRoomForItem::ItemComponent does not have a valid Item Category."))
		return FInv_SlotAvailabilityResult();
	}
}

void UInv_SpatialInventory::OnItemHovered(UInv_InventoryItem* Item)
{
    const auto& Manifest = Item->GetItemManifest();
	UInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, DescriptionWidget]()
	{
            Manifest.AssimilateInventoryFragments(DescriptionWidget);
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
        // Assimilate the manifest data into the item description widget.
	});

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UInv_SpatialInventory::OnItemUnhovered()
{
    GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
    GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UInv_SpatialInventory::HasHoveredItem() const
{
    if (Grid_Equippables->HasHoverItem()) return true;
    if (Grid_Consumables->HasHoverItem()) return true;
    if (Grid_Craftables->HasHoverItem()) return true;
    if (Grid_Quest->HasHoverItem()) return true;
    if (Grid_ItemCategory01->HasHoverItem()) return true;
    if (Grid_ItemCategory02->HasHoverItem()) return true;
    if (Grid_ItemCategory03->HasHoverItem()) return true;
	return false;
}

UInv_HoverItem* UInv_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	
	return ActiveGrid->GetHoverItem();
}

float UInv_SpatialInventory::GetTileSize() const
{
    return Grid_Equippables->GetTileSize();
}

UInv_ItemDescription* UInv_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void UInv_SpatialInventory::ShowAllItems()
{
	SetActiveGrid(Grid_All, Button_All);
}

void UInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventory::ShowQuestItems()
{
	SetActiveGrid(Grid_Quest, Button_Quest);
}

void UInv_SpatialInventory::ShowItemCategory01()
{
	SetActiveGrid(Grid_ItemCategory01, Button_ItemCategory01);
}

void UInv_SpatialInventory::ShowItemCategory02()
{
	SetActiveGrid(Grid_ItemCategory02, Button_ItemCategory02);
}

void UInv_SpatialInventory::ShowItemCategory03()
{
	SetActiveGrid(Grid_ItemCategory03, Button_ItemCategory03);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
		ActiveGrid->OnHide();
	}
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);

	Switcher->SetActiveWidget(Grid);
}

void UInv_SpatialInventory::DisableButton(UButton* Button)
{
	Button_All->SetIsEnabled(true);
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button_Quest->SetIsEnabled(true);
	Button_ItemCategory01->SetIsEnabled(true);
	Button_ItemCategory02->SetIsEnabled(true);
	Button_ItemCategory03->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

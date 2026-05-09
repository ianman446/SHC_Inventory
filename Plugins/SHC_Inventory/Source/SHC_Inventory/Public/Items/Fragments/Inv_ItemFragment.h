// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "Inv_ItemFragment.generated.h"

class APlayerController;
class UInv_CompositeBase;

USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
    GENERATED_BODY()

    FInv_ItemFragment() {}
    /// <summary>
    /// Rule of 5
    /// </summary>
    /// <param name=""></param>
    FInv_ItemFragment(const FInv_ItemFragment&) = default;              // Constructor
    FInv_ItemFragment& operator=(const FInv_ItemFragment&) = default;   // Constructor
    FInv_ItemFragment(FInv_ItemFragment&&) = default;                   // Move operation
    FInv_ItemFragment& operator=(FInv_ItemFragment&&) = default;        // Move operation
    virtual ~FInv_ItemFragment() {}                                     // Destructor

    FGameplayTag GetFragmentTag() const { return FragmentTag; }
    void SetFragmentTag(FGameplayTag Tag) { FragmentTag = Tag; }

    virtual void Manifest() {}


private:

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="FragmentTags"))
    FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

/*
* Item Fragment specifically for assimilation into a widget.
*/



USTRUCT(BlueprintType)
struct FInv_InventoryItemFragment : public FInv_ItemFragment
{
    GENERATED_BODY()

    virtual void Assimilate(UInv_CompositeBase* Composite) const;

protected:

    bool MatchesWidgetTag(const UInv_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
{
    GENERATED_BODY()

public:
    FIntPoint GetGridSize() const { return GridSize; }
    void SetGridSize(FIntPoint Size) { GridSize = Size; }

    float GetGridPadding() const { return GridPadding; }
    void SetGridPadding(float Padding) { GridPadding = Padding; }

private:

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FIntPoint GridSize = (1,1);

    UPROPERTY(EditAnywhere, Category = "Inventory")
    float GridPadding = 0.f;
};

USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_InventoryItemFragment
{
    GENERATED_BODY()

    UTexture2D* GetIcon() const { return Icon; }
    void SetIcon(UTexture2D* InIcon) { Icon = InIcon; }
    virtual void Assimilate(UInv_CompositeBase* Composite) const;

    FVector2D GetIconDimensions() const { return IconDimensions; }
    void SetIconDimentions(FVector2D Dimentions) { IconDimensions = Dimentions; }

private:
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TObjectPtr<UTexture2D> Icon = nullptr;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FVector2D IconDimensions = FVector2D(44.f, 44.f);
};

USTRUCT(BlueprintType)
struct FInv_TextFragment : public FInv_InventoryItemFragment
{
    GENERATED_BODY()

    FText GetText() const { return FragmentText; }
    void SetText(const FText& InText) { FragmentText = InText; }
    virtual void Assimilate(UInv_CompositeBase* Composite) const override;

private:

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FText FragmentText;
};

USTRUCT(BlueprintType)
struct FInv_LabeledNumberFragment : public FInv_InventoryItemFragment
{
    GENERATED_BODY()

    virtual void Assimilate(UInv_CompositeBase* Composite) const override;
    virtual void Manifest() override;
    float GetFloatValue() const { return Float_Value; }

    // When manifesting for the first time, this fragment will randomize. Then it will be set to false.
    bool bRandomizeOnManifest = true;

private:

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FText Text_Label{};

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    float Float_Value = 0.f;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    float MinValue = 0.f;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    float MaxValue = 100.f;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    bool bCollapseLabel = false;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    bool bCollapseValue = false;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    int32 MinFractionalDigits = 1;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    int32 MaxFractionalDigits = 1;
};

USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
    GENERATED_BODY()

    int32 GetMaxStackSize() const { return MaxStackSize; }
    void SetMaxStackSize(int32 MaxSize);

    int32 GetStackCount() const { return StackCount; }
    void SetStackCount(int32 CurrentCount);

private:

    UPROPERTY(EditAnywhere, Category = "Inventory")
    int32 MaxStackSize = 1;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    int32 StackCount = 1;

};

USTRUCT(BlueprintType)
struct FInv_ConsumeModifier : public FInv_LabeledNumberFragment
{
    GENERATED_BODY()

    virtual void OnConsume(APlayerController* PC) {};
};

//Parent Fragment for all Consumable Item Fragments. Contains OnConsume function which can be implemented by child fragments to add functionality when consuming an item.
USTRUCT(BlueprintType)
struct FInv_ConsumableFragment : public FInv_InventoryItemFragment
{
    GENERATED_BODY()

    virtual void OnConsume(APlayerController* PC);
    virtual void Assimilate(UInv_CompositeBase* Composite) const override;
    virtual void Manifest() override;

private:

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
    TArray<TInstancedStruct<FInv_ConsumeModifier>> ConsumeModifiers;

};

USTRUCT(BlueprintType)
struct FInv_HealthPotionFragment : public FInv_ConsumeModifier
{
    GENERATED_BODY()

    virtual void OnConsume(APlayerController* PC) override;

};

USTRUCT(BlueprintType)
struct FInv_ManaPotionFragment : public FInv_ConsumeModifier
{
    GENERATED_BODY()

    virtual void OnConsume(APlayerController* PC) override;

};

// Equipment
//

USTRUCT(BlueprintType)
struct FInv_EquipModifier : public FInv_LabeledNumberFragment
{
    GENERATED_BODY()

    virtual void OnEquip(APlayerController* PC) {}
    virtual void OnUnequip(APlayerController* PC) {}

};

USTRUCT(BlueprintType)
struct FInv_StrengthModifier : public FInv_EquipModifier
{
    GENERATED_BODY()

    virtual void OnEquip(APlayerController* PC) override;
    virtual void OnUnequip(APlayerController* PC) override;

};

class AInv_EquipActor;
USTRUCT(BlueprintType)
struct FInv_EquipmentFragment : public FInv_InventoryItemFragment
{
    GENERATED_BODY()

    bool bEquipped{ false };
    void OnEquip(APlayerController* PC);
    void OnUnequip(APlayerController* PC);
    virtual void Assimilate(UInv_CompositeBase* Composite) const override;
    virtual void Manifest() override;

    AInv_EquipActor* SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const;
    void DestroyAttachedActor() const;
    FGameplayTag GetEquipmentType() const { return EquipmentType; }
    void SetEquippedActor(AInv_EquipActor* EquipActor);

private:

    UPROPERTY(EditAnywhere, Category = "Inventory")
    TArray<TInstancedStruct<FInv_EquipModifier>> EquipModifiers;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<AInv_EquipActor> EquipActorClass = nullptr;

    TWeakObjectPtr<AInv_EquipActor> EquippedActor = nullptr;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FName SocketAttachPoint{ NAME_None };

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
};


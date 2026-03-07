// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Inv_ItemFragment.generated.h"

class APlayerController;

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

private:

    UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="FragmentTags"))
    FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
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
struct FInv_ImageFragment : public FInv_ItemFragment
{
    GENERATED_BODY()

    UTexture2D* GetIcon() const { return Icon; }
    void SetIcon(UTexture2D* InIcon) { Icon = InIcon; }

    FVector2D GetIconDimensions() const { return IconDimensions; }
    void SetIconDimentions(FVector2D Dimentions) { IconDimensions = Dimentions; }

private:
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TObjectPtr<UTexture2D> Icon = nullptr;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    FVector2D IconDimensions = FVector2D(44.f, 44.f);
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

//Parent Fragment for all Consumable Item Fragments. Contains OnConsume function which can be implemented by child fragments to add functionality when consuming an item.
USTRUCT(BlueprintType)
struct FInv_ConsumableFragment : public FInv_ItemFragment
{
    GENERATED_BODY()

    virtual void OnConsume(APlayerController* PC) {}; 

};

USTRUCT(BlueprintType)
struct FInv_HealthPotionFragment : public FInv_ConsumableFragment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Inventory")
    float HealAmount = 50.f;

    virtual void OnConsume(APlayerController* PC) override;

};

USTRUCT(BlueprintType)
struct FInv_ManaPotionFragment : public FInv_ConsumableFragment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Inventory")
    float ManaAmount = 50.f;

    virtual void OnConsume(APlayerController* PC) override;

};
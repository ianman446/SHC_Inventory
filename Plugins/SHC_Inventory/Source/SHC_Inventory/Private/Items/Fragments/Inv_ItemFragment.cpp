// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Fragments/Inv_ItemFragment.h"

void FInv_StackableFragment::SetMaxStackSize(int32 MaxSize)
{
    if (MaxSize <= 0) MaxSize = 1;
    MaxStackSize = MaxSize;
}

void FInv_StackableFragment::SetStackCount(int32 CurrentCount)
{
    if (CurrentCount < 0) CurrentCount = 0;
    StackCount = CurrentCount;
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
    //GAS THIS BITCH UP

    //BS for now
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health Potion Consumed! \n Healing by %f"), HealAmount));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
    //GAS THIS BITCH UP

//BS for now
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mana Potion Consumed! \n Regening by %f"), ManaAmount));
}

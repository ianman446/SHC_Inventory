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
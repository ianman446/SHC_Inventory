// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/Inv_Leaf_Text.h"

#include "Components/TextBlock.h"

void UInv_Leaf_Text::SetText(const FText& Text)
{
    Text_LeafText->SetText(Text);
}

void UInv_Leaf_Text::NativePreConstruct()
{
    Super::NativeConstruct();

    FSlateFontInfo FontInfo = Text_LeafText->GetFont();
    FontInfo.Size = FontSize;
    Text_LeafText->SetFont(FontInfo);
}

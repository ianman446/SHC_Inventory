// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Inv_HighlightablleStaticMesh.h"

void UInv_HighlightablleStaticMesh::Highlight_Implementation()
{
    SetOverlayMaterial(HighlightMaterial);
}

void UInv_HighlightablleStaticMesh::UnHighlight_Implementation()
{
    SetOverlayMaterial( nullptr );
}

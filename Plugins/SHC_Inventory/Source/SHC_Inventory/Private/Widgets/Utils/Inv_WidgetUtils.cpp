// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"

FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UInv_WidgetUtils::IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X >= BoundaryPos.X && MousePos.X <= (BoundaryPos.X + WidgetSize.X) &&
		MousePos.Y >= BoundaryPos.Y && MousePos.Y <= (BoundaryPos.Y + WidgetSize.Y);
}

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
    FVector2D ClampedPosition = MousePos;

    if (MousePos.X + WidgetSize.X > Boundary.X) // Does the widget go out of bounds on the right side?
	{
        ClampedPosition.X = Boundary.X - WidgetSize.X; //IF so, clamp it to the right edge of the boundary
	}
    else if (MousePos.X < 0.f) // Does the widget go out of bounds on the left side?
	{
        ClampedPosition.X = 0.f; // If so, clamp it to the left edge of the boundary
    }

	if (MousePos.Y + WidgetSize.Y > Boundary.Y) // Does the widget go out of bounds on the bottom side?
	{
		ClampedPosition.Y = Boundary.Y - WidgetSize.Y; // If so, clamp it to the bottom edge of the boundary
	}
	else if (MousePos.Y < 0.f) // Does the widget go out of bounds on the top side?
	{
        ClampedPosition.Y = 0.f; // If so, clamp it to the top edge of the boundary
	}

	return ClampedPosition;
}

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

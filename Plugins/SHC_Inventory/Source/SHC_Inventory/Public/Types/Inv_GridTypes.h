#pragma once

#include "Inv_GridTypes.generated.h"

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
    All,
    Equippable,
    Consumable,
    Craftable,
    Quest,
    ItemCategory01,
    ItemCategory02,
    ItemCategory03,
    None
};

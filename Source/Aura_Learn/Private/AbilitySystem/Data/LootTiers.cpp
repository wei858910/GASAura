﻿#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
    TArray<FLootItem> ReturnItems;

    for (const auto& Item : LootItems)
    {
        for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
        {
            if (FMath::RandRange(1, 100) < Item.ChanceToSpawn)
            {
                FLootItem NewItem;
                NewItem.LootClass = Item.LootClass;
                NewItem.bLootLevelOverride = Item.bLootLevelOverride;
                ReturnItems.Add(NewItem);
            }
        }
    }

    return ReturnItems;
}
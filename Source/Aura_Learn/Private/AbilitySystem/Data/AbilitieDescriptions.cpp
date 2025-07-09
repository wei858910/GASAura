#include "AbilitySystem/Data/AbilitieDescriptions.h"

const FText& UAbilitieDescriptions::FindDescriptionsByTagAndLevel(const FGameplayTag& AbilityTag, const int32 Level)
{
    for (const auto& it : AbilityDescriptionInfos)
    {
        if (it.AbilityTag.MatchesTagExact(AbilityTag))
        {
            if (Level >= it.AbilityDescriptions.Num())
            {
                return it.AbilityDescriptions.Last();
            }
            return it.AbilityDescriptions[Level];
        }
    }
    return AbilityDescriptionInfos[0].AbilityDescriptions[0];
}
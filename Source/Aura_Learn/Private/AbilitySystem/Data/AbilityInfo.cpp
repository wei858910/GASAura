#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura_Learn/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    for (auto Info : AbilityInfomation)
    {
        if (Info.AbilityTag == AbilityTag)
        {
            return Info;
        }
    }

    if (bLogNotFound)
    {
        UE_LOG(LogAura, Warning, TEXT("Can't Find AbilityInfo AbilityTag [%s] On [%s]"), *AbilityTag.ToString(), *GetNameSafe(this))
    }
    return FAuraAbilityInfo();
}
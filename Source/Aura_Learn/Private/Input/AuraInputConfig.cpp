// 学习使用

#include "Input/AuraInputConfig.h"
#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
    for (const auto& InputActionInfo : AbilityInputActions)
    {
        if (InputActionInfo.InputTag.MatchesTag(Tag) && IsValid(InputActionInfo.InputAction))
        {
            return InputActionInfo.InputAction;
        }
    }

    if (bLogNotFound)
        UE_LOG(LogTemp, Warning, TEXT("Not Found Tag [%s] Form [%s] or Null InputAction in DataAsset"), *Tag.ToString(), *GetName())

    return nullptr;
}
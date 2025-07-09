#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

class UAuraAbilitySystemComponent;
/**
 * 
 */

UCLASS()
class AURA_LEARN_API UAuraGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="初始输入Tag", meta = (Categories = "InputTag"))
    FGameplayTag StartupInputTag;

    virtual FString GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag, const int32 Level); //获取技能的文本描述
    static FString GetLockedDescription(const int32 Level);
};
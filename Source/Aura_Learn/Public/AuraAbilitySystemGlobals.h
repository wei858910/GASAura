// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
    GENERATED_BODY()

    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override; //原本new了FGameplayEffectContext 改成自己的context
};
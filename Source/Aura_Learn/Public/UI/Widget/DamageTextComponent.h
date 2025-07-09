// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UDamageTextComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetDamageText(float Damage, const bool bBlocked = false, const bool bCriticalHit = false);
};
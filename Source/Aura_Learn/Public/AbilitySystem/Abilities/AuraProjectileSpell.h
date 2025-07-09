// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/* 所有飞行类技能 */
UCLASS()
class AURA_LEARN_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()

public:
    virtual FString GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag, const int32 Level) override;

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UFUNCTION(BlueprintCallable, Category="Projectile")
    void SpawnProjectile(const FVector& TargetLocation, bool bOveridePitch = false, float PitchOverride = 0.f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AAuraProjectile> ProjectfileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "发射物最大数量")
    int32 NumProjectiles{ 5 }; //最大发射火焰数量
};
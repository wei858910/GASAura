// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;

/**
 *
 */
UCLASS()
class AURA_LEARN_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
    GENERATED_BODY()

public:
    AAuraEnemy();
    /* HighlightInterface */
    virtual void HighlightActor_Implementation() override;
    virtual void UnHightlightActor_Implementation() override;
    virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
    /* HighlightInterface */

    FORCEINLINE virtual int32 GetPlayerLevel_Implementation() override;

    virtual void Die(const FVector& DeathImpulse) override;

    virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

    virtual AActor* GetCombatTarget_Implementation() override;
    void SetEnemyLevel(int32 Inlevel) { Level = Inlevel; };

    UFUNCTION(BlueprintImplementableEvent)
    void SpawnLoot();

protected:
    virtual void BeginPlay() override;
    virtual void InitAbilityActorInfo() override;
    virtual void InitDefaultAttribute() const override;

    void HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCounter);

    virtual void PossessedBy(AController* NewController) override;

    virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount) override;

public:
    UPROPERTY(BlueprintReadOnly)
    bool bHightlighted{ false };

    UPROPERTY(BlueprintReadOnly)
    bool bHitReacting{ false };

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
    int32 Level{ 1 };

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnHealthChangedDel;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnMaxHealthChangedDel;

    UPROPERTY(EditAnywhere, Category="AI", DisplayName="行为树")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    UPROPERTY()
    TObjectPtr<AAuraAIController> AuraAIController;

    UPROPERTY(BlueprintReadWrite, Category = "战斗")
    TObjectPtr<AActor> CombatTarget{ nullptr }; //对战目标

};
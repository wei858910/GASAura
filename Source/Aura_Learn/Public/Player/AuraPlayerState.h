#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangeDel, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChangeDel, uint16, bool /*是否是升级*/);

UCLASS()
class AURA_LEARN_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AAuraPlayerState();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    UAttributeSet* GetAttributeSet() const { return AttributeSet; };

    FORCEINLINE int16 GetPlayerLevel() const;
    FORCEINLINE int32 GetPlayerXP() const;
    FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
    FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

    UFUNCTION(BlueprintCallable)
    void SetXP(const int32 InXP);

    UFUNCTION(BlueprintCallable)
    void AddToXP(const int32 InXP);

    UFUNCTION(BlueprintCallable)
    void SetLevel(const int32 InLevel); //不触发升级提示

    UFUNCTION(BlueprintCallable)
    void AddToLevel(const int32 InLevel);

    void AddToAttributePoints(int32 InPoints);
    void AddToSpellPoints(int32 InPoints);

    FOnPlayerStateChangeDel OnXPChangeDel;
    FOnLevelChangeDel OnLevelChangeDel;
    FOnPlayerStateChangeDel OnAttributePointsChangedDelegate;
    FOnPlayerStateChangeDel OnSpellPointsChangedDelegate;

    UPROPERTY(EditDefaultsOnly, DisplayName="升级信息资产表")
    TObjectPtr<ULevelUpInfo> DA_LevelInfo;

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

private:
    UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
    int16 Level{ 1 };

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
    int32 XP{ 0 };


    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
    int32 AttributePoints{ 0 };

    UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
    int32 SpellPoints{ 0 };

    UFUNCTION()
    void OnRep_Level(int16 OldValue);

    UFUNCTION()
    void OnRep_XP(int32 OldValue);

    UFUNCTION()
    void OnRep_AttributePoints(int32 OldAttributePoints);

    UFUNCTION()
    void OnRep_SpellPoints(int32 OldSpellPoints);

};
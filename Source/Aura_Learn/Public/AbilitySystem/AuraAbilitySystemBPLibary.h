#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "AuraAbilitySystemBPLibary.generated.h"

class UAbilityInfo;
class AAuraHUD;
class USpellMenuWgtController;
class UCombatInterface;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
struct FWidgetControllerParams;
class UAttributeMenuWgtController;
class UOverlayWidgetController;

UCLASS()
class AURA_LEARN_API UAuraAbilitySystemBPLibary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//为了能访问世界的某些内容，最好拿到世界上下文
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLib|WgtController",meta=(DefaultToSelf="WorldContenxt"))
	static UOverlayWidgetController* GetOverlayWgtController(const UObject* WorldContenxt);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|WgtController", meta = (DefaultToSelf = "WorldContenxt"))
	static UAttributeMenuWgtController* GetAttributeMenuWgtController(const UObject* WorldContenxt);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|WgtController", meta = (DefaultToSelf = "WorldContenxt"))
	static USpellMenuWgtController* GetSpellMenuWgtController(const UObject* WorldContenxt);

	static bool SetWgtCtrlParamsByWorldContenxt(const UObject* WorldContenxt,OUT FWidgetControllerParams& WgtCtrlParam,AAuraHUD*& OutAuraHud);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLib|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContenxt, ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC);//从CharacterClassInfo的配置获取信息初始化属性

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContenxt, UAbilitySystemComponent* ASC,const ECharacterClass CharacterClass);//从CharacterClassInfo的配置获取初始的能力

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContenx);//透过GameMode 访问CharacterClassInfo资产管理

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static const FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);//该GE中伤害是否被格挡

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);//该GE中伤害是否暴击

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)/*非常量引用会被认为是节点输出参数，用该方式标识该变量是输入参数*/FGameplayEffectContextHandle& EffectContextHandle,const bool Value);//设置GE中伤害是否被格挡

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle,const bool Value);//设置GE中伤害是否暴击

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsActiveHitReact(const FGameplayEffectContextHandle& EffectContextHandle);//是否启用受击反应
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetActiveHitReact(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const bool bIsActive);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContext, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& IgnoreActors,
	                                     const FVector& Center, float Radius = 100.f); //寻找半径内的存活玩家

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "AuraAbilitySystemLib|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* OtherActor,const FName Tag="Player");//根据两者Acotr是否有同一Tag表示是否为对友

	UFUNCTION() //从目标职业类型和等级获取其奖励的经验值
	static int32 GetXpRewardForClassAndLevel(const UObject* WorldContext, ECharacterClass ClassType, const int16 Level);

	UFUNCTION()
	static const FText& GetAbilityDescriptionByLevel(const UObject* WorldContext, const FGameplayTag& GATag, const int32 GALevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|DamageEffect")
	static TMap<FGameplayTag, FGameplayEffectContextHandle> ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);//从参数里面获取的参数向目标应用伤害GE

	/**
	 * 生成一组能构成扇形的旋转
	 * @param Forward 向前向量
	 * @param Axis 扇形的绕该轴旋转生成
	 * @param Spread 扇形总共展开的角度
	 * @param Count 生成的数量
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLib|GameplayMechanics")
	static TArray<FRotator> EvenlySpeacedRotators(const FVector& Forward, const FVector& Axis,const float Spread,const int32 Count);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLib|GameplayMechanics")
	static TArray<FVector> EvenlySpeacedFVector(const FVector& Forward, const FVector& Axis, const float Spread, const int32 Count);//生成一组扇形向量

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayMechanics")
	static void GetClosesTarget(const FVector& Origin,int32 MaxTarget, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosesTargets);//从一组Actor中挑出最近的N个Actor
};

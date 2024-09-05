// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemBPLibary.generated.h"

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
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLib|WgtController")
	static UOverlayWidgetController* GetOverlayWgtController(const UObject* WorldContenxt);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|WgtController")
	static UAttributeMenuWgtController* GetAttributeMenuWgtController(const UObject* WorldContenxt);

	static bool SetWgtCtrlParamsByWorldContenxt(const UObject* WorldContenxt,OUT FWidgetControllerParams& WgtCtrlParam);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLib|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContenxt, ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC);//从CharacterClassInfo的配置获取信息初始化属性

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContenxt, UAbilitySystemComponent* ASC,const ECharacterClass CharacterClass);//从CharacterClassInfo的配置获取初始的能力

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContenx);//透过GameMode 访问CharacterClassInfo资产管理

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);//该GE中伤害是否被格挡

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLib|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);//该GE中伤害是否暴击

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)/*非常量引用会被认为是节点输出参数，用该方式标识该变量是输入参数*/FGameplayEffectContextHandle& EffectContextHandle,const bool Value);//设置GE中伤害是否被格挡

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle,const bool Value);//设置GE中伤害是否暴击

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLib|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContext, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& IgnoreActors,
	                                     const FVector& Center, float Radius = 100.f); //寻找半径内的存活玩家

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "AuraAbilitySystemLib|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* OtherActor,const FName Tag="Player");//根据两者Acotr是否有同一Tag表示是否为对友
};

// 学习使用

#include "AbilitySystem/AuraAbilitySystemBPLibary.h"

#include <memory>

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemBPLibary::GetOverlayWgtController(const UObject* WorldContenxt)
{
	FWidgetControllerParams WgtControllerParams{};
	AAuraHUD* AuraHUD{ nullptr };
	bool Sucesses= SetWgtCtrlParamsByWorldContenxt(WorldContenxt, WgtControllerParams, AuraHUD);
	if (!Sucesses)return nullptr; 

	return AuraHUD->GetOverlaiController(WgtControllerParams);
}

UAttributeMenuWgtController* UAuraAbilitySystemBPLibary::GetAttributeMenuWgtController(const UObject* WorldContenxt)
{
	FWidgetControllerParams WgtControllerParams{};
	AAuraHUD* AuraHUD{ nullptr };
	bool Sucesses = SetWgtCtrlParamsByWorldContenxt(WorldContenxt, WgtControllerParams, AuraHUD);
	if (!Sucesses)return nullptr;

	return AuraHUD->GetAtributeMenuController(WgtControllerParams);
}

USpellMenuWgtController* UAuraAbilitySystemBPLibary::GetSpellMenuWgtController(const UObject* WorldContenxt)
{
	FWidgetControllerParams WgtControllerParams{};
	AAuraHUD* AuraHUD{ nullptr };
	bool Sucesses = SetWgtCtrlParamsByWorldContenxt(WorldContenxt, WgtControllerParams, AuraHUD);
	if (!Sucesses)return nullptr;

	return AuraHUD->GetSpellMenuController(WgtControllerParams);
}

bool UAuraAbilitySystemBPLibary::SetWgtCtrlParamsByWorldContenxt(const UObject* WorldContenxt,
                                                                 OUT FWidgetControllerParams& WgtCtrlParam,
                                                                 AAuraHUD*& OutAuraHud)
{
	auto Controller = UGameplayStatics::GetPlayerController(WorldContenxt, 0); //该函数在本地调用，0一定是玩家自己的控制器
	if (!IsValid(Controller))return false;

	auto PS = Controller->GetPlayerState<AAuraPlayerState>();
	if (!IsValid(PS))return false;

	auto ASC = PS->GetAbilitySystemComponent();
	auto AttributeSet = PS->GetAttributeSet();
	WgtCtrlParam.PlayerController = Controller;
	WgtCtrlParam.PlayerState = PS;
	WgtCtrlParam.AbilitySystemComponent = ASC;
	WgtCtrlParam.AttributeSet = AttributeSet;
	OutAuraHud = Cast<AAuraHUD>(WgtCtrlParam.PlayerController->GetHUD());

	if (!IsValid(OutAuraHud))return false;
	return true;
}

void UAuraAbilitySystemBPLibary::InitializeDefaultAttributes(const UObject* WorldContenxt, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	auto ClassDefaultInfo = GetCharacterClassInfo(WorldContenxt);
	if (!IsValid(ClassDefaultInfo))return;

	auto GEContext = ASC->MakeEffectContext();
	GEContext.AddSourceObject(ASC->GetAvatarActor());
	
	auto PrimaryAttributeSpecHandl = ASC->MakeOutgoingSpec(ClassDefaultInfo->GetClassDefaultInfo(CharacterClass).PrimaryAttributes, Level, GEContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandl.Data.Get());

	auto SecondaryAttributeSpecHandl = ASC->MakeOutgoingSpec(ClassDefaultInfo->SecondaryAttributes, Level, GEContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandl.Data.Get());

	auto ViatalAttributeSpecHandl = ASC->MakeOutgoingSpec(ClassDefaultInfo->ViatalAttributes, Level, GEContext);
	ASC->ApplyGameplayEffectSpecToSelf(*ViatalAttributeSpecHandl.Data.Get());
}

void UAuraAbilitySystemBPLibary::GiveStartupAbilities(const UObject* WorldContenxt, UAbilitySystemComponent* ASC, const ECharacterClass CharacterClass)
{

	auto ClassDefaultInfo = GetCharacterClassInfo(WorldContenxt);
	if (!IsValid(ClassDefaultInfo))return;

	//获取通用能力
	for(const auto& it:ClassDefaultInfo->CommonAbilities)
	{
		FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(it, 1.f);
		ASC->GiveAbility(GASpec);
	}

	//获取该类型特有的能力
	const auto& TargetDefaultInfo=ClassDefaultInfo->GetClassDefaultInfo(CharacterClass);
	for(const auto& Abilities:TargetDefaultInfo.StartupAbilities)
	{
		if(auto CombatInterface=Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			
			FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(Abilities, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(GASpec);

		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemBPLibary::GetCharacterClassInfo(const UObject* WorldContenxt)
{
	auto GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContenxt));
	if (!IsValid(GameMode))return nullptr;

	return GameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemBPLibary::GetAbilityInfo(const UObject* WorldContext)
{
	auto GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!IsValid(GameMode))return nullptr;

	return GameMode->AbilityInfo;
}

bool UAuraAbilitySystemBPLibary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	
	if (auto GEContext = static_cast<const FAuraGameEffectContext*>(EffectContextHandle.Get()))
		return GEContext->IsBlockedHit();
	return false;
}

bool UAuraAbilitySystemBPLibary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (auto GEContext = static_cast<const FAuraGameEffectContext*>(EffectContextHandle.Get()))
		return GEContext->IsCriticalHit();
	return false;
}

void UAuraAbilitySystemBPLibary::GetLivePlayersWithRadius(const UObject* WorldContext, TArray<AActor*>& OutOverlappingActors,
                                                          const TArray<AActor*>& IgnoreActors, const FVector& Center, float Radius)
{
	/*无非就是生成个检测圆，进行范围内检测,筛选后装载*/

	//碰撞查询参数
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	if (auto World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Results;
		World->OverlapMultiByObjectType(Results, Center, FQuat::Identity,
		                                FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), /*所有动态物体*/
		                                FCollisionShape::MakeSphere(Radius), QueryParams);

		for (const auto& HitRes : Results)
		{
			if (!HitRes.GetActor()->Implements<UCombatInterface>())continue; //Actor是否实现了接口？
			if (ICombatInterface::Execute_IsDead(HitRes.GetActor()))continue; //挂了就挂了
			if (IgnoreActors.Contains(HitRes.GetActor()))continue;//不是忽略对象

			OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(HitRes.GetActor()));
		}
	}
}

bool UAuraAbilitySystemBPLibary::IsNotFriend(const AActor* FirstActor, const AActor* OtherActor, const FName Tag)
{
	if(IsValid(FirstActor)&&IsValid(OtherActor))
	{
		return !(FirstActor->ActorHasTag(Tag) && OtherActor->ActorHasTag(Tag));
	}
	return false;
}

int32 UAuraAbilitySystemBPLibary::GetXpRewardForClassAndLevel(const UObject* WorldContext, ECharacterClass ClassType, const int16 Level)
{
	auto ClassDefaultInfo = GetCharacterClassInfo(WorldContext);
	if (!ClassDefaultInfo)return 0;

	const auto& DefaultInfo = ClassDefaultInfo->GetClassDefaultInfo(ClassType);

	return (int32)DefaultInfo.XPReward.GetValueAtLevel(Level);
}

void UAuraAbilitySystemBPLibary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool Value)
{
	if (auto GEContext = static_cast<FAuraGameEffectContext*>(EffectContextHandle.Get()))
	{
		GEContext->SetBlockedHit(Value);
	}
}

void UAuraAbilitySystemBPLibary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool Value)
{
	if (auto GEContext = static_cast<FAuraGameEffectContext*>(EffectContextHandle.Get()))
	{
		GEContext->SetCriticalHit(Value);
	}
}

#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* BeDamageActor)
{
	auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.F);
	for(const auto& Pair:DamageTypes)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, Pair.Value.GetValueAtLevel(GetAbilityLevel()));
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(BeDamageActor));
}

FGameplayTag UAuraDamageGameplayAbility::GetCurrentAbilityTag() const
{
	for(const auto& GATag: GetAssetTags())
	{
		if (GATag.MatchesTag(FAuraGmaeplayTags::GetInstance().Abilities))return GATag;
	}
	return FAuraGmaeplayTags::GetInstance().Abilities_None;
}

const TMap<FGameplayTag, FScalableFloat>& UAuraDamageGameplayAbility::GetDamageTypes() const
{
	return DamageTypes;
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor)
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.AbilityLevel = GetAbilityLevel();
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	Params.bApplyHitReact = bIsApplyHitReact;

	if(bIsApplyHitReact)
	{
		const bool bKnockback = FMath::RandRange(1, 100) < KnockbackChance;
		if (IsValid(TargetActor) && bKnockback)
		{
			FRotator Rotation{ 0.f };
			if (bIsRadialDamage) //如果有径向伤害，击退按径向源计算
			{
				Rotation = (TargetActor->GetActorLocation() - RadialDamageOrigin).Rotation();
			}
			else
			{
				Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
			}
			const FVector ToTarget = Rotation.Vector();
			Rotation.Pitch = 45.f;
			Params.DeathImpulseVectro = ToTarget * DeathImpulseMagnitude;
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
			Params.KnockbackChance = 100;
		}else if(!bKnockback)//计算出不应用击退
		{
			Params.KnockbackChance = 0;
			Params.bApplyHitReact = false;
		}
	}

	/*径向伤害*/
	if(bIsRadialDamage)
	{
		Params.bIsRadialDamage = true;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = RadialDamageOrigin;
	}

	MakeDeBuffGEParams(Params.DebuffMapGEParams);
	return Params;
}

/*
 * debuff 的应用机会与等级和该属性伤害有关 伤害占大头
 * debuff 的伤害仅与该类型的伤害有关
 * 如果想必发DEbuff 则在技能之中直接应用
*/

void UAuraDamageGameplayAbility::MakeDeBuffGEParams(TMap<FGameplayTag, FDamageGEParamsByDamageType>& DebuffMapParams)
{
	UAuraAbilitySystemComponent* AuraGAS = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

	if (!AuraGAS)return;
	//计算debuff应用几率
	int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(GetAvatarActorFromActorInfo());
	 bool bIsHero{ false };
	if(auto CombatIF=Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		bIsHero=CombatIF->IsHeroCharacter();
	}

	for(const auto& it :DamageTypes)
	{
		const auto DamageValue = DamageTypes[it.Key].GetValueAtLevel(GetAbilityLevel());
		auto Func = AuraGAS->DamageTypeMapSetParamFunction.Find(it.Key);
		if (Func)
		{
			FGameplayTag DebuffTag = FAuraGmaeplayTags::GetInstance().DamageTypesToDebuffs[it.Key];
			DebuffMapParams.Emplace(DebuffTag, (*Func)(PlayerLevel, GetAbilityLevel(), DamageValue, bIsHero));
		}
		
	}

}

const FTaggedMontage& UAuraDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& Arrary) const
{
	check(Arrary.Num())
	return Arrary[FMath::RandRange(0, Arrary.Num() - 1)];
}

float UAuraDamageGameplayAbility::GetManaCost(const float Level) const
{
	float ManaCost{ 0.f };
	const auto CostGE = GetCostGameplayEffect(); //获取GA中花费GE类
	if (!CostGE)return ManaCost;


	for (const auto& ModifierInfo : CostGE->Modifiers)
	{
		////对Mana修改的修饰器 那一定是法力花销值
		if (ModifierInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
		{
			ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
			break;
		}
	}

	return ManaCost;
}


float UAuraDamageGameplayAbility::GetCooldown(const float Level) const
{
	float CD{ 0.F };
	const auto CDGE = GetCooldownGameplayEffect();
	if (!CDGE)return CD;

	//CD 是持续时间的属性修改器中
	CDGE->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, CD);

	return CD;
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(const FGameplayTag& DamageType, const float Level) const
{
	if (!DamageTypes.Contains(DamageType))return 0.f;
	return DamageTypes[DamageType].GetValueAtLevel(Level);
}

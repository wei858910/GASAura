#include "AbilitySystem/Abilities/Electrocute.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UElectrocute::GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag,
                                     const int32 Level)
{
	if (!AuraGAS)return FString{};

	const int32 Damage = GetDamageByDamageType(FAuraGmaeplayTags::GetInstance().Damage_Lightning, Level);
	FText CostofMana = FText::FromString(FString::Printf(TEXT("%.1f"), GetManaCost(Level)));
	FText CoolDown = FText::FromString(FString::Printf(TEXT("%.1f"), GetCooldown(Level)));

	auto Description = FText::Format(UAuraAbilitySystemBPLibary::GetAbilityDescriptionByLevel(AuraGAS->GetAvatarActor(), GATag, Level), CostofMana, CoolDown, Level, Damage,FMath::Min(Level-1,MaxNumShockTargets));
	return Description.ToString();
}

#include "AbilitySystem/Abilities/AuraFireBlast.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitieDescriptions.h"

FString UAuraFireBlast::GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag, const int32 Level)
{
	if (!AuraGAS)return FString{};

	const int32 Damage = GetDamageByDamageType(FAuraGmaeplayTags::GetInstance().Damage_Fire, Level);
	FText CostofMana = FText::FromString(FString::Printf(TEXT("%.1f"), GetManaCost(Level)));
	FText CoolDown = FText::FromString(FString::Printf(TEXT("%.1f"), GetCooldown(Level)));

	auto Description = FText::Format(AuraGAS->AbilityDescriptions->FindDescriptionsByTagAndLevel(GATag, Level), FMath::Min(Level, NumFireBalls), Damage, Level, CostofMana, CoolDown);
	return Description.ToString();
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBall()
{
	TArray<AAuraFireBall*>FireBalls;
	return FireBalls;
}

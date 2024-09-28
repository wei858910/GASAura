// 学习使用


#include "AbilitySystem/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UAuraGameplayAbility::GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag,const int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"默认技能名字 - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetLockedDescription(const int32 Level)
{
	return FString{};
}

float UAuraGameplayAbility::GetManaCost(const float Level)
{
	return 0.f;
}

float UAuraGameplayAbility::GetCooldown(const float Level)
{
	return 0.f;
}

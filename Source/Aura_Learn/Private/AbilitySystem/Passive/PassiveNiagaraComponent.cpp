#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	if(auto AuraASC=Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivatePassiveEffectDel.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
	}else
	{
		//采用回调的方式，等待GAS有效时再进行绑定
		if (auto CombatIF = Cast<ICombatInterface>(GetOwner()))
		{
			CombatIF->GetOnASCRegisteredDel().AddLambda([this](UAbilitySystemComponent* InAuraASC)
			{
				if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
					UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
				{
					AuraASC->ActivatePassiveEffectDel.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
				}
			});
		}
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if(AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if(bActivate &&!IsActive())
		{
			Activate();
		}else
		{
			Deactivate();
		}

	}
}

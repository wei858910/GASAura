#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
    bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
    Super::BeginPlay();

    auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
    auto CombatInterface = Cast<ICombatInterface>(GetOwner());

    if (ASC)
    {
        ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagCahnged);
    }
    else if (CombatInterface) //如果未有效，则靠代理通知，有效时再绑定
    {
        CombatInterface->GetOnASCRegisteredDel().AddWeakLambda(this, [this](UAbilitySystemComponent* ASCTemp) {
            ASCTemp->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagCahnged);
        });
    }

}

void UDebuffNiagaraComponent::DebuffTagCahnged(const FGameplayTag CallackTag, const int32 NewCount)
{
    const bool bOwnerValid = IsValid(GetOwner());
    const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());

    if (NewCount > 0 && bOwnerValid && bOwnerAlive)
    {
        Activate();
    }
    else
    {
        Deactivate();
    }
}

void UDebuffNiagaraComponent::OnOnwerDead(AActor* DeadActor)
{
    Deactivate();
}
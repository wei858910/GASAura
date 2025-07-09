// 学习使用


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    PlayerController = WCParams.PlayerController;
    PlayerState = WCParams.PlayerState;
    AbilitySystemComponent = WCParams.AbilitySystemComponent;
    AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues() {}

void UAuraWidgetController::BindCallbackToDependencies() {}

void UAuraWidgetController::BroadcastAbilityInfo()
{
    if (!GetAuraASC())
    {
        return;
    }

    FForEachAbility LoopBordGADel;
    LoopBordGADel.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec) {
        const FGameplayTag AbilityTag = UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
        if (!AbilityTag.IsValid())
        {
            return;
        }

        auto AbilityInfoTemp = AbilityInfo->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
        AbilityInfoTemp.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
        AbilityInfoTemp.StatusTag = UAuraAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
        AbilityInfoDel.Broadcast(AbilityInfoTemp);
    });

    GetAuraASC()->ForEachAbility(LoopBordGADel);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
    if (AuraPlayerController == nullptr)
    {
        AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
    }
    return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
    if (AuraPlayerState == nullptr)
    {
        AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
    }
    return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
    if (AuraAbilitySystemComponent == nullptr)
    {
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
    }
    return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
    if (AuraAttributeSet == nullptr)
    {
        AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
    }
    return AuraAttributeSet;
}
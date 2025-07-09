// 学习使用


#include "UI/WidgetController/AttributeMenuWgtController.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWgtController::BindCallbackToDependencies()
{
    auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    //将每个 属性 的变化 代理 绑定到 对应的广播上
    for (auto& Info : AttributeInfo.Get()->AttributeInformation)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            AttributeInfo->FindAttributeInfoForTag(Info.AttributeTag).AttributeGetter).AddLambda(
            [this, Info](const FOnAttributeChangeData& Data) {
                BroadcastAttributeInfo(Info.AttributeTag);
            });
    }

    auto AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
    AuraPS->OnAttributePointsChangedDelegate.AddLambda([this](const int32 NewPoint) {
        AttributePointsChangedDelegate.Broadcast(NewPoint);
    });
}

void UAttributeMenuWgtController::BroadcastInitialValues()
{
    if (!IsValid(AttributeInfo))
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] AttributeInfo Not Setup"), *this->GetName())
        return;
    }

    auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    //在这个DA资产中 循环获取每个 FAuratributeInfo
    for (const auto& Info : AttributeInfo.Get()->AttributeInformation)
    {
        BroadcastAttributeInfo(Info.AttributeTag);
    }

    auto AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
    AttributePointsChangedDelegate.Broadcast(AuraPS->GetAttributePoints());
}

void UAttributeMenuWgtController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
    auto Info = AttributeInfo->FindAttributeInfoForTag(Tag);

    Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet); //从属性获取当前值

    OnAttributeInfoChangeDel.Broadcast(Info);

}

void UAttributeMenuWgtController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
    if (!IsValid(AuraASC))
    {
        return;
    }
    AuraASC->UpgradeAttribute(AttributeTag);
}
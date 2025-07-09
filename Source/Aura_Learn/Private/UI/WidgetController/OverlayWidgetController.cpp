// 学习使用
#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
    auto AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

    OnHealthChangedDel.Broadcast(AuraAS->GetHealth());
    OnMaxHealthChangedDel.Broadcast(AuraAS->GetMaxHealth());
    OnMaxManaChangedDel.Broadcast(AuraAS->GetMaxMana());
    OnManaChangedDel.Broadcast(AuraAS->GetMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
    auto AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);

    BindAttributeChangeHelperFunc(AuraAS->GetHealthAttribute(), OnHealthChangedDel);
    BindAttributeChangeHelperFunc(AuraAS->GetMaxHealthAttribute(), OnMaxHealthChangedDel);
    BindAttributeChangeHelperFunc(AuraAS->GetManaAttribute(), OnManaChangedDel);
    BindAttributeChangeHelperFunc(AuraAS->GetMaxManaAttribute(), OnMaxManaChangedDel);

    if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        /**
         * 因为时序上的问题，当ASC组件已经获取初始化技能时，不需要委托代理执行
         * 但是但ASC 未添加完成初始化时，不应该直接调用BroadcastAbilityInfo，等待ASC的代理调用即可 
         */
        if (AuraASC->bStartupAbilitiesGiven)
        {
            BroadcastAbilityInfo();
        }
        else
        {
            //没有获取到初始技能，则等代理进行调用
            AuraASC->AbilitiesGiveDel.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
        }

        AuraASC->EffectAssetTagsDel.AddLambda([this](const FGameplayTagContainer& AssetTags) {
            for (auto& Tag : AssetTags)
            {
                auto MessageTag = FGameplayTag::RequestGameplayTag("Message");
                if (Tag.MatchesTag(MessageTag)) //TagA.MatchesTag(TagB) 判断A是否为B的子标签
                {
                    auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
                    if (Row)
                    {
                        MessageWidgetRowDel.Broadcast(*Row);
                    }
                }
            }
        });

        AuraASC->AbilityEquippedDel.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
    }

    auto AuraPS = Cast<AAuraPlayerState>(PlayerState);
    if (AuraPS && IsValid(AuraPS->DA_LevelInfo))
    {
        AuraPS->OnXPChangeDel.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

        AuraPS->OnLevelChangeDel.AddLambda([this](const int32 NewLevel, const bool ShowMsg) {
            OnPlayerLevelChangedDel.Broadcast(NewLevel, ShowMsg);
        });
    }
}

/*
 * 1.FOnAttributeChangedSignature 应该之后被改为一个代理通知变化 因此 适时再重启该函数
 * 2.使用方式如下 属性集目标 管理目标的代理
 *   - BindAttributeChangeHelperFunc （AttributeSet -> GetHealthAttribute （），OnHealthChanged ）； 
 */
void UOverlayWidgetController::BindAttributeChangeHelperFunc(FGameplayAttribute Attribute,
    FOnAttributeChangedSignature& AttributeData)
{
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
        [this, &AttributeData](const FOnAttributeChangeData& Data) {
            AttributeData.Broadcast(Data.NewValue);
        });
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{

    auto AuraPS = Cast<AAuraPlayerState>(PlayerState);
    if (AuraPS &&
        IsValid(AuraPS->DA_LevelInfo) &&
        /*等级不超过表内最大等级*/(AuraPS->GetPlayerLevel() <= AuraPS->DA_LevelInfo->LevelUpInformation.Num())
    )
    {
        auto TaragetInforation = AuraPS->DA_LevelInfo->LevelUpInformation; //拿到信息数组
        auto LastLevelXP = 0;

        if (AuraPS->GetPlayerLevel() >= 2) //Index0就是1级,因此上一级 的索引为-2
        {
            LastLevelXP = TaragetInforation[AuraPS->GetPlayerLevel() - 2].LevelUpRequirement; //上一次升了级的XP
        }
        float TotalDelta = TaragetInforation[AuraPS->GetPlayerLevel() - 1].LevelUpRequirement - LastLevelXP; //该次升级所需的XP-上次所需的
        float CurrentDelat = AuraPS->GetPlayerXP() - LastLevelXP;
        OnXPPercentChangedDel.Broadcast(CurrentDelat / TotalDelta); //通知经验条变化
    }

}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& GATag, const FGameplayTag& StatusTag,
    const FGameplayTag& TargetSlot, const FGameplayTag& PrevSlot)
{
    FAuraAbilityInfo LastSlotInfo;
    LastSlotInfo.StatusTag = FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked;
    LastSlotInfo.InputTag = PrevSlot;
    LastSlotInfo.AbilityTag = FAuraGmaeplayTags::GetInstance().Abilities_None;

    //如果装备的技能 的上一个整备槽有效 则广播 LastSlotInfo
    AbilityInfoDel.Broadcast(LastSlotInfo);

    //更新现在的信息
    auto GAInfo = AbilityInfo->FindAbilityInfoForTag(GATag);
    GAInfo.InputTag = TargetSlot;
    GAInfo.StatusTag = StatusTag;
    AbilityInfoDel.Broadcast(GAInfo);

}
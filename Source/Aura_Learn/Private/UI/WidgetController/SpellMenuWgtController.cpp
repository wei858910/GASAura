#include "UI/WidgetController/SpellMenuWgtController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWgtController::BroadcastInitialValues()
{
    BroadcastAbilityInfo();

    if (auto AuraPS = GetAuraPS())
    {
        SpellPointChangeDel.Broadcast(AuraPS->GetSpellPoints());
    }
}

void USpellMenuWgtController::BindCallbackToDependencies()
{
    GetAuraASC()->AbilityStatusChangeDel.AddLambda([this](const FGameplayTag& GATag, const FGameplayTag& StatusTag, int32 GALevel) {
        //技能界面的按钮状态由选择时更改，如果选择未改变 但是状态改变了，也得进行广播通知
        if (GATag.MatchesTagExact(SelectAbility.Ability))
        {
            SelectAbility.Status = StatusTag;

            //技能状态改变得通知界面按钮
            UpdateAbilityWgtStatus(GATag, StatusTag, CurrentSpellPoint);

        }

        if (AbilityInfo)
        {
            auto GAInfo = AbilityInfo->FindAbilityInfoForTag(GATag);
            GAInfo.StatusTag = StatusTag;
            AbilityInfoDel.Broadcast(GAInfo);
        }
    });

    GetAuraASC()->AbilityEquippedDel.AddUObject(this, &USpellMenuWgtController::OnAbilityEquipped);

    if (auto AuraPS = GetAuraPS())
    {
        AuraPS->OnSpellPointsChangedDelegate.AddLambda([this](const int32 SpellPoint) {
            SpellPointChangeDel.Broadcast(SpellPoint);
            CurrentSpellPoint = SpellPoint;

            //技能点改变也得通知界面按钮更新状态
            UpdateAbilityWgtStatus(SelectAbility.Ability, SelectAbility.Status, CurrentSpellPoint);
        });
    }

}

void USpellMenuWgtController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
    //停止技能装备栏的动画
    const auto TypeTag = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
    StopWaitingEquipAnim(TypeTag);

    const int32 SpellPoint = GetAuraPS()->GetSpellPoints();

    const bool bTagValid = AbilityTag.IsValid();
    const bool bTagNone = AbilityTag.MatchesTag(FAuraGmaeplayTags::GetInstance().Abilities_None); //是否是占位技能标签
    auto GASpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
    const bool bSpecValid = GASpec != nullptr; //找到可激活Tag

    FGameplayTag AbilityStatus;
    if (!bTagValid || bTagNone || !bSpecValid) //会出现未解锁的情况
    {
        AbilityStatus = FAuraGmaeplayTags::GetInstance().Abilities_Status_Locked;
    }
    else //其他情况技能已解锁 从GAS读取技能现有规格
    {
        AbilityStatus = GetAuraASC()->GetStatusFromSpec(*GASpec);
    }

    //缓存此次选中时的状态
    SelectAbility.Ability = AbilityTag;
    SelectAbility.Status = AbilityStatus;

    //判断按钮启用状态
    UpdateAbilityWgtStatus(AbilityTag, AbilityStatus, SpellPoint);
}

void USpellMenuWgtController::SpendPointBtnPressed()
{
    if (!GetAuraASC())
    {
        return;
    }
    GetAuraASC()->ServerSpendSpellPoint(SelectAbility.Ability);
}

void USpellMenuWgtController::GlobeDeSelect()
{
    //技能装备栏动画停止
    const auto TypeTag = AbilityInfo->FindAbilityInfoForTag(SelectAbility.Ability).AbilityType;
    StopWaitingEquipAnim(TypeTag);

    SelectAbility.Ability = FAuraGmaeplayTags::GetInstance().Abilities_None;
    SelectAbility.Status = FAuraGmaeplayTags::GetInstance().Abilities_Status_Locked;
    FString Empty{ "" };
    SpellGlobeSelectedDel.Broadcast(false, false, Empty, Empty);
}

void USpellMenuWgtController::EquipBtnPressed()
{
    const auto AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectAbility.Ability).AbilityType;

    WaitForEquipSelectionDel.Broadcast(AbilityType);
    bWaitingForEquipSelection = true;

    const auto SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectAbility.Ability);

    //已经装备就获取装备的输入Tag
    if (SelectedStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped))
    {
        SelectedSlot = GetAuraASC()->GetSlotTagFromAbilityTag(SelectAbility.Ability);
    }
}

void USpellMenuWgtController::ShouldEnableBtns(const FGameplayTag& AbilityStatus, int32 SpellPoints,
    bool& bShouldEnableSpellPointBtn, bool& bShouldEnbalEquipBtn)
{
    //技能解锁了都可以花点升级 但是装备按钮仅能在解锁和装备状态下启用
    if (AbilityStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped))
    {
        bShouldEnbalEquipBtn = true;
        bShouldEnableSpellPointBtn = SpellPoints > 0;
    }
    else if (AbilityStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Eligible))
    {
        bShouldEnbalEquipBtn = false;
        bShouldEnableSpellPointBtn = SpellPoints > 0;
    }
    else if (AbilityStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked))
    {
        bShouldEnbalEquipBtn = true;
        bShouldEnableSpellPointBtn = SpellPoints > 0;
    }
    else if (AbilityStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Locked))
    {
        bShouldEnbalEquipBtn = false;
        bShouldEnableSpellPointBtn = false;
    }
}

void USpellMenuWgtController::UpdateAbilityWgtStatus(const FGameplayTag& GATag, const FGameplayTag& StatusTag, const int32 SpellPoint)
{
    bool bShouldEnableSpellPointBtn{ false };
    bool bShouldEnableEquipBtn{ false };
    ShouldEnableBtns(StatusTag, SpellPoint, bShouldEnableSpellPointBtn, bShouldEnableEquipBtn);

    FString Description{ "" }, NextLevelDescription{ "" };
    GetAuraASC()->GetDescriptionByAbilityTag(GATag, Description, NextLevelDescription);

    SpellGlobeSelectedDel.Broadcast(bShouldEnableSpellPointBtn, bShouldEnableEquipBtn, Description, NextLevelDescription);
}

void USpellMenuWgtController::StopWaitingEquipAnim(const FGameplayTag& TypeTag)
{
    if (bWaitingForEquipSelection)
    {
        bWaitingForEquipSelection = false;
        StopWaitForEquipSelectionDel.Broadcast(TypeTag);
    }
}

void USpellMenuWgtController::SpellRowGlobePressed(const FGameplayTag& ClickedSlotTag, const FGameplayTag& ClickedTypeTag)
{
    if (!bWaitingForEquipSelection)
    {
        return; //没有进入装配状态，那还装配什么？
    }

    //根据输入标签将技能装入对应的输入球，更改技能的输入按钮。注意主动和被动不能混用
    const auto SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectAbility.Ability).AbilityType;

    if (!ClickedTypeTag.MatchesTagExact(SelectedAbilityType)) //选中的技能和点击的技能栏球不是一个技能类型
    {
        return;
    }

    GetAuraASC()->ServerEquipAbility(SelectAbility.Ability, ClickedSlotTag);
}

void USpellMenuWgtController::OnAbilityEquipped(const FGameplayTag& GATag, const FGameplayTag& StatusTag, const FGameplayTag& TargetSlot,
    const FGameplayTag& PrevSlot)
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

    StopWaitForEquipSelectionDel.Broadcast(GAInfo.AbilityType);

    bWaitingForEquipSelection = false;

    SpellGlobeReassignedDel.Broadcast(GATag);

    GlobeDeSelect();
}
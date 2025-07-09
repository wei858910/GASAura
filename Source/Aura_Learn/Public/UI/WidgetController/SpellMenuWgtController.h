#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWgtController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedDel, const bool, bSpendPointBtnEnabled, const bool, bEquipBtnEnabaled, const FString&, DescriptionStr, const FString&, NextLevelDescription);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionDel, const FGameplayTag&, Abilitytype); //按下整备技能按钮

struct FSelectAbility
{
    FGameplayTag Ability{};
    FGameplayTag Status{};
};

UCLASS(BlueprintType, Blueprintable)
class AURA_LEARN_API USpellMenuWgtController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbackToDependencies() override;

    UFUNCTION(BlueprintCallable)
    void SpellGlobeSelected(const FGameplayTag& AbilityTag); //界面中的技能球被选中时触发

    UFUNCTION(BlueprintCallable)
    void SpendPointBtnPressed();

    UFUNCTION(BlueprintCallable)
    void GlobeDeSelect();

    UFUNCTION(BlueprintCallable)
    void EquipBtnPressed(); //按下技能配备按钮

    UPROPERTY(BlueprintAssignable)
    FWaitForEquipSelectionDel WaitForEquipSelectionDel;

    UPROPERTY(BlueprintAssignable)
    FWaitForEquipSelectionDel StopWaitForEquipSelectionDel;

    UPROPERTY(BlueprintAssignable)
    FWaitForEquipSelectionDel SpellGlobeReassignedDel; //复用同参数代理 当技能完成分配时发出 参数为技能标签

    UPROPERTY(BlueprintAssignable)
    FOnPlayerStateChangedSignature SpellPointChangeDel;

    UPROPERTY(BlueprintAssignable)
    FSpellGlobeSelectedDel SpellGlobeSelectedDel;

protected:
    //根据技能状态和技能点数判断界面按钮启用状态
    static void ShouldEnableBtns(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointBtn, bool& bShouldEnbalEquipBtn);
    void UpdateAbilityWgtStatus(const FGameplayTag& GATag, const FGameplayTag& StatusTag, const int32 SpellPoint); //广播按钮启用状态和新的描述文本
    void StopWaitingEquipAnim(const FGameplayTag& TypeTag);                                                        //停止技能装配栏的动画

    UFUNCTION(BlueprintCallable)
    void SpellRowGlobePressed(const FGameplayTag& ClickedSlotTag, const FGameplayTag& ClickedTypeTag); //技能装配栏的按钮被点击

    void OnAbilityEquipped(const FGameplayTag& GATag, const FGameplayTag& StatusTag, const FGameplayTag& TargetSlot, const FGameplayTag& PrevSlot); //技能装配
private:
    //缓存上次选择的技能 如果其他地方导致技能状态改变，依照这个缓存 更新内容 进行广播通知界面按钮
    FSelectAbility SelectAbility{ FAuraGmaeplayTags::GetInstance().Abilities_None, FAuraGmaeplayTags::GetInstance().Abilities_Status_Locked };
    int32 CurrentSpellPoint{ 0 };

    bool bWaitingForEquipSelection{ false }; //是否正在等待装备技能
    FGameplayTag SelectedSlot;               //选中的技能的输入Tag
};
#pragma once
#include <functional>

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AuraAbilitySystemComponent.generated.h"

struct FAuraGmaeplayTags;
class ULoadScreenSaveGame;
class UAuraDamageGameplayAbility;
class UAbilitieDescriptions;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&/*AssetTags*/)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenDel)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangeDel, const FGameplayTag&/*GATag*/, const FGameplayTag&/*StatusTag*/, int32/*技能等级*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipedDel, const FGameplayTag&/*GATag*/, const FGameplayTag&/*StatusTag*/, const FGameplayTag&/*TargetSlot*/, const FGameplayTag&/*PrevSlot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeActivePassiveAbilityDel, const FGameplayTag&/*GA Tag*/);                           //结束被动技能
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffectDel, const FGameplayTag& /*AbilityTag*/, bool b/*bActivate*/); //激活被动对应粒子

UCLASS()
class AURA_LEARN_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    void AbilityActorInfoSeted(); //AvatarActor和OwnerActor 设置完成之后的调用

    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
    void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);
    void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities); //被动技能添加,会激活一次

    /**
     * 接入GAS的输入主要方便一些GAS的事件调用
     */
    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagHeld(const FGameplayTag& InputTag);     //查找到可以激活的GA 触发GA的按住函数 如果GA未在激活中 则激活GA
    void AbilityInputTagReleased(const FGameplayTag& InputTag); //查找可激活的GA 触发GA的释放操作函数

    void ForEachAbility(const FForEachAbility& Delegate); //使入参代理进行广播该ASC中每个有效的技能

    static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& GASpec); //从技能中找到目标的技能Tag
    static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& GASpec);   //从技能中找到目标输入Tag
    static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& GASpec);     //从技能获取解锁状态

    FGameplayTag GetSlotTagFromAbilityTag(const FGameplayTag& AbilityTag);       //从技能标签返回输入Tag
    FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);        //从技能标签返回技能状态Tag
    FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag); //从技能Tag获取技能规格

    bool SlotIsEmpty(const FGameplayTag& Slot);                                                              //对应技能插槽空着吗？
    static bool AbilitySetupedSlot(const FGameplayTag& Slot, const FGameplayAbilitySpec& TargetAbilitySpec); //目标技能是否已经设置到插槽上？
    FGameplayAbilitySpec* GetAbilitySpecWithSlot(const FGameplayTag& Slot);                                  //获取该槽上的技能
    static bool IsPassiveAbility(const FGameplayAbilitySpec& TargetAbilitySpec);                             //技能是否为被动技能？

    void UpgradeAttribute(const FGameplayTag& AttributeTag); //根据属性标签升级对应属性

    /**
     * Server 此函数仅在服务器上执行
     */
    UFUNCTION(Server, Reliable)
    void ServerUpgradeAttribute(const FGameplayTag& AttributeTag); //RPC 升级属性

    void UpdateAbilityStatus(const int32 Level); //更新技能的状态

    UFUNCTION(Server, Reliable)
    void ServerSpendSpellPoint(const FGameplayTag& AbilityTag); //由服务端进行技能点花费

    UFUNCTION(Server, Reliable)
    void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag); //装备技能得通知下服务器

    /*仅仅是视觉效果，因此可以不可靠*/
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate); //被动技能生效时，其粒子效果广播

    UFUNCTION(Client, Reliable)
    void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& TargetSlotTag/*整备进的槽位*/, const FGameplayTag& PrevSlotTag/*之前的槽位*/); //装备技能客户端处理的部分

    bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OUT Description, FString& OUT NextLevelDescription); //由技能标签获取技能描述

    static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);                                                                     //技能是否在目标技能槽？
    static bool AbilityHasAnySlot(FGameplayAbilitySpec* Spec);                                                                                            //技能是否在任何一技能插槽？
    void ClearAndAssignSlotToAbility(FGameplayAbilitySpec& GASpec, const FGameplayTag& NewSlot);                                                          //清除技能原来插槽，赋予新插槽
    void ClearSlotAndChangeStatus(FGameplayAbilitySpec* GASpec, const FGameplayTag& Status = FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked); //清除技能的输入标签槽
    void ClearAbilitiesOfSlot(const FGameplayTag& SlotTag);                                                                                               //根据槽标签清除技能上的槽标签

    TMap<FGameplayTag, std::function<FDamageGEParamsByDamageType(const int16 PlayerLevel, const int16 AbilityLevel, const float BaseDamage, const bool bIsHero)>> DamageTypeMapSetParamFunction; //伤害类型映射对应的Debuff参数填充
protected:
    //被GE应用自身时的广播绑定
    UFUNCTION(Client, Reliable) //仅在拥有在其上调用此函数的对象的客户端上执行，函数命名存在套路；此函数将通过网络复制
    void ClinetEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle);

    UFUNCTION(Client, Reliable)
    void ClientUpdateAbilityStatus(const FGameplayTag& GATag, const FGameplayTag& StatusTag, int32 GALevel = 1);

    virtual void OnRep_ActivateAbilities() override;

public:
    FEffectAssetTags EffectAssetTagsDel; //用以广播GE的AssetTags

    FAbilitiesGivenDel AbilitiesGiveDel; //技能添加时进行广播的代理

    FAbilityStatusChangeDel AbilityStatusChangeDel; //技能状态改变时的代理

    FAbilityEquipedDel AbilityEquippedDel; //技能装配

    FDeActivePassiveAbilityDel DeActivePassiveAbilityDel; //结束被动技能

    FActivatePassiveEffectDel ActivatePassiveEffectDel; //被动技能对应粒子效果启停

    bool bStartupAbilitiesGiven{ false }; //初始技能是否已经获取

    UPROPERTY()
    TObjectPtr<UAbilitieDescriptions> AbilityDescriptions;
};
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "AbilitySystem/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura_Learn/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystem/Data/AbilitieDescriptions.h"
#include "Game/LoadScreenSaveGame.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSeted()
{
    AbilityDescriptions = LoadObject<UAbilitieDescriptions>(nullptr, TEXT("/Game/Blueprints/AbilitySystem/Data/DA_AbiilityDescriptionInfos"));

    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClinetEffectApplied);

    auto GetChance = [](const int16 PlayerLevel, const int16 GALevel, const float Damage, const bool bIsHero)-> float {
        int32 GALevelChance = GALevel;
        if (!bIsHero)
        {
            GALevelChance = FMath::Max(GALevelChance / 3, 1);
        }
        //if (Damage < 10)return 0.f; //如果不想触发，则让伤害小于10
        float Coefficient = (Damage - 9) * 0.2 + PlayerLevel * 0.5 + (GALevelChance - 1) * 6;
        //系数算下来在 0-1之间 *100 进行扩张
        return (Coefficient / (Coefficient + 100)) * 100;
    };

    DamageTypeMapSetParamFunction.Emplace(FAuraGmaeplayTags::GetInstance().Damage_Fire, [GetChance](const int16 PlayerLevel, const int16 AbilityLevel, const float BaseDamage, const bool bIsHero)-> FDamageGEParamsByDamageType {
        FDamageGEParamsByDamageType Info{};

        Info.DamageType = FAuraGmaeplayTags::GetInstance().Damage_Fire;
        Info.BaseDamage = BaseDamage;
        Info.DebuffDamage = (BaseDamage / 10.f) + AbilityLevel;
        Info.DebuffDuration = (AbilityLevel * 5 + PlayerLevel * 0.5) / 8 + Info.DebuffDamage + 2;
        Info.DebuffChance = GetChance(PlayerLevel, AbilityLevel, BaseDamage, bIsHero);
        return Info;
    });

    DamageTypeMapSetParamFunction.Emplace(FAuraGmaeplayTags::GetInstance().Damage_Arcane, [GetChance](const int16 PlayerLevel, const int16 AbilityLevel, const float BaseDamage, const bool bIsHero)-> FDamageGEParamsByDamageType {
        FDamageGEParamsByDamageType Info{};

        Info.DamageType = FAuraGmaeplayTags::GetInstance().Damage_Arcane;
        Info.BaseDamage = BaseDamage;
        Info.DebuffDamage = (BaseDamage / 6.f) + AbilityLevel;
        Info.DebuffDuration = (AbilityLevel * 2 + PlayerLevel * 0.5) / 10 + Info.DebuffDamage + 2;
        Info.DebuffChance = GetChance(PlayerLevel, AbilityLevel, BaseDamage, bIsHero);
        return Info;
    });

    DamageTypeMapSetParamFunction.Emplace(FAuraGmaeplayTags::GetInstance().Damage_Physical, [GetChance](const int16 PlayerLevel, const int16 AbilityLevel, const float BaseDamage, const bool bIsHero)-> FDamageGEParamsByDamageType {
        FDamageGEParamsByDamageType Info{};

        Info.DamageType = FAuraGmaeplayTags::GetInstance().Damage_Physical;
        Info.BaseDamage = BaseDamage;
        Info.DebuffDamage = (BaseDamage / 8.f) + AbilityLevel;
        Info.DebuffDuration = (AbilityLevel * 2 + PlayerLevel * 0.5) / 7 + Info.DebuffDamage + 3;
        Info.DebuffChance = GetChance(PlayerLevel, AbilityLevel, BaseDamage, bIsHero);
        return Info;
    });

    DamageTypeMapSetParamFunction.Emplace(FAuraGmaeplayTags::GetInstance().Damage_Lightning, [GetChance](const int16 PlayerLevel, const int16 AbilityLevel, const float BaseDamage, const bool bIsHero)-> FDamageGEParamsByDamageType {
        FDamageGEParamsByDamageType Info{};

        Info.DamageType = FAuraGmaeplayTags::GetInstance().Damage_Lightning;
        Info.BaseDamage = BaseDamage;
        Info.DebuffDamage = (BaseDamage / 10.f) + AbilityLevel / 2.f;
        Info.DebuffDuration = FMath::Min((AbilityLevel * 4 + PlayerLevel * 0.5) / 10 + Info.DebuffDamage / 5 + 1, 8.f);
        Info.DebuffChance = GetChance(PlayerLevel, AbilityLevel, BaseDamage, bIsHero);
        return Info;
    });
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{

    for (auto& AbilityClass : Abilities)
    {
        auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
        if (const auto AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);                               //将输入Tag添加进DynamicAbilityTags
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped); //已经装备
            GiveAbility(AbilitySpec);
        }
    }
    bStartupAbilitiesGiven = true;
    AbilitiesGiveDel.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
    for (auto& AbilityData : SaveData->SaveAbilities)
    {
        auto AbilitySpec = FGameplayAbilitySpec(AbilityData.GameplayAbility, AbilityData.AbilityLevel);

        AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilitySlot); //将输入Tag添加进DynamicAbilityTags
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilityStatus);

        GiveAbility(AbilitySpec);

        //已经装备的被动技能需要激活
        if (AbilityData.AbilityType == FAuraGmaeplayTags::GetInstance().Abilities_Type_Passive
            && AbilityData.AbilityStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped))
        {
            TryActivateAbility(AbilitySpec.Handle);
        }

    }

    bStartupAbilitiesGiven = true;
    AbilitiesGiveDel.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
    for (auto& AbilityClass : PassiveAbilities)
    {
        auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped);
        GiveAbilityAndActivateOnce(AbilitySpec);
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }

    FScopedAbilityListLock ActiveScopedLock(*this); //上锁
    //遍历 拿到可以激活的GA
    for (auto& GASpec : GetActivatableAbilities()) //GetActivatableAbilities返回的变量有客户端收到回调的函数版本，因此客户端的需要的相应内容需要在onrep函数处理
    {
        if (GASpec.GetDynamicSpecSourceTags().HasTag(InputTag))
        {

            AbilitySpecInputPressed(GASpec); //告知GA 目前操作状态在 Pressed下

            if (GASpec.IsActive()) //激活状态才进行下一步调用，用于抬起施法类
            {
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GASpec.Handle, GASpec.ActivationInfo.GetActivationPredictionKey());
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }
    FScopedAbilityListLock ActiveScopedLock(*this); //上锁
    //遍历 拿到可以激活的GA
    for (auto& GASpec : GetActivatableAbilities()) //GetActivatableAbilities返回的变量有客户端收到回调的函数版本，因此客户端的需要的相应内容需要在onrep函数处理
    {
        if (GASpec.GetDynamicSpecSourceTags().HasTag(InputTag))
        {

            AbilitySpecInputPressed(GASpec); //告知GA 目前操作状态在 Pressed下

            if (!GASpec.IsActive()) //非激活状态才进行激活 防止输入每帧调用
            {
                TryActivateAbility(GASpec.Handle); //激活
            }
        }
    }

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }
    FScopedAbilityListLock ActiveScopedLock(*this); //上锁
    //遍历 拿到可以激活的GA
    for (auto& GASpec : GetActivatableAbilities())
    {
        if (GASpec.GetDynamicSpecSourceTags().HasTag(InputTag) && GASpec.IsActive())
        {

            AbilitySpecInputReleased(GASpec);                                                                                                        //告知GA 目前操作状态在 已经取消
            InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GASpec.Handle, GASpec.ActivationInfo.GetActivationPredictionKey()); //本地回调已注册的通用事件 这里是按键松绑
        }
    }
}

void UAuraAbilitySystemComponent::ClinetEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle GameplayEffectHandle)
{
    FGameplayTagContainer TagContainer;
    GameplayEffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTagsDel.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopedLock(*this); //临时锁定能力列表，防止能力列表在执行特定操作时发生变化,是自动锁机制
    for (const auto& AbilitySpec : GetActivatableAbilities())
    {
        //执行代理的函数，在UOverlayWidgetController::OnInitStartupAbilities 中 代理的函数设置为根据能力Tag查找DA获取能力相关的UI信息，并且进行广播
        if (!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogAura, Warning, TEXT("Failed to Excute delegate in %hs "), __FUNCTION__);
        }
    }
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& GASpec)
{
    if (!IsValid(GASpec.Ability))
    {
        return FGameplayTag{};
    }
    for (const auto& Tag : GASpec.Ability->GetAssetTags())
    {
        if (Tag.MatchesTag(FAuraGmaeplayTags::GetInstance().Abilities))
        {
            return Tag;
        }
    }
    return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& GASpec)
{
    if (!IsValid(GASpec.Ability))
    {
        return FGameplayTag{};
    }
    for (const auto& Tag : GASpec.GetDynamicSpecSourceTags()) //与该能力实例相关的 动态 Gameplay Tags
    {
        if (Tag.MatchesTag(FAuraGmaeplayTags::GetInstance().InputTag))
        {
            return Tag;
        }
    }
    return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& GASpec)
{
    if (!IsValid(GASpec.Ability))
    {
        return FGameplayTag{};
    }
    for (const auto& Tag : GASpec.GetDynamicSpecSourceTags()) //与该能力实例相关的 动态 Gameplay Tags
    {
        if (Tag.MatchesTag(FAuraGmaeplayTags::GetInstance().Abilities_Status))
        {
            return Tag;
        }
    }
    return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if (auto Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetInputTagFromSpec(*Spec);
    }
    return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
    if (auto Spec = GetSpecFromAbilityTag(AbilityTag))
    {
        return GetStatusFromSpec(*Spec);
    }
    return FGameplayTag{};
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopedLock(*this); //上锁

    //从可激活的所有GA内的Tag中找到对应技能Tag
    for (auto& GASpec : GetActivatableAbilities())
    {
        for (const auto& GATag : GASpec.Ability->GetAssetTags())
        {
            if (AbilityTag.MatchesTagExact(GATag))
            {
                return &GASpec;
            }

        }
    }

    return nullptr;
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
    FScopedAbilityListLock ActiveScopedLock(*this); //上锁
    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySetupedSlot(Slot, AbilitySpec))
        {
            return false;
        }

    }

    return true;
}

bool UAuraAbilitySystemComponent::AbilitySetupedSlot(const FGameplayTag& Slot, const FGameplayAbilitySpec& TargetAbilitySpec)
{
    return TargetAbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecWithSlot(const FGameplayTag& Slot)
{
    FScopedAbilityListLock ActiveScopedLock(*this); //上锁

    for (auto& AbilitySpec : GetActivatableAbilities())
    {
        if (AbilitySetupedSlot(Slot, AbilitySpec))
        {
            return &AbilitySpec;
        }

    }
    return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& TargetAbilitySpec)
{
    return GetAbilityTagFromSpec(TargetAbilitySpec).MatchesTag(FAuraGmaeplayTags::GetInstance().Abilities_Passive);
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    if (GetAvatarActor()->Implements<UPlayerInterface>())
    {
        if (IPlayerInterface::Execute_GetCurrentAttributePoint(GetAvatarActor()) > 0)
        {
            ServerUpgradeAttribute(AttributeTag); //主要是为了防止客户端没有点数调用服务器执行而浪费资源，因此先让客户端判断，再通知服务器升级
        }
    }
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus(const int32 Level)
{
    auto AbilityInfo = UAuraAbilitySystemBPLibary::GetAbilityInfo(GetAvatarActor());

    for (const auto& Info : AbilityInfo->AbilityInfomation)
    {
        if (Level < Info.LevelRequirement || !Info.AbilityTag.IsValid())
        {
            continue;
        }

        //等级够 并且 非可激活状态(GAS 中没有该技能) 才进行更改
        if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
        {
            auto FutureAddGASpec = FGameplayAbilitySpec(Info.Ability, 1);
            FutureAddGASpec.GetDynamicSpecSourceTags().AddTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Eligible);
            GiveAbility(FutureAddGASpec);

            MarkAbilitySpecDirty(FutureAddGASpec); //标记能力规格已被修改 强制现在更新

            ClientUpdateAbilityStatus(Info.AbilityTag, FAuraGmaeplayTags::GetInstance().Abilities_Status_Eligible); //客户端广播技能状态改变
        }
    }
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
    if (auto GASpec = GetSpecFromAbilityTag(AbilityTag))
    {
        const auto PrevSlot = GetInputTagFromSpec(*GASpec); //该技能之前的 输入槽位
        const auto Status = GetStatusFromSpec(*GASpec);

        //技能状态是能操作的 已解锁||已经整备
        const bool bIsValidStatus =
            Status.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked) ||
            Status.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped);

        if (bIsValidStatus)
        {

            /*插槽不空 清除原有技能 并且停止被动技能*/
            if (!SlotIsEmpty(SlotTag))
            {
                if (auto SlotAbilitySpec = GetAbilitySpecWithSlot(SlotTag))
                {
                    if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SlotAbilitySpec))) //同个插槽同个技能 不做处理
                    {
                        ClientEquipAbility(AbilityTag, SlotTag, PrevSlot);
                        return;
                    }

                    /*被动技能的启停*/
                    if (IsPassiveAbility(*SlotAbilitySpec))
                    {
                        MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SlotAbilitySpec), false);
                        DeActivePassiveAbilityDel.Broadcast(GetAbilityTagFromSpec(*SlotAbilitySpec));
                    }

                    ClearSlotAndChangeStatus(SlotAbilitySpec); //清除该技能的插槽
                }

            }

            //之前没有插槽，所以如果是被动技能 之前也没有激活
            if (!AbilityHasAnySlot(GASpec))
            {
                if (IsPassiveAbility(*GASpec))
                {
                    TryActivateAbility(GASpec->Handle);
                    MulticastActivatePassiveEffect(AbilityTag, true);
                }
                GASpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusFromAbilityTag(AbilityTag));
                GASpec->GetDynamicSpecSourceTags().AddTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped);
            }

            //将槽位赋予给该技能
            ClearAndAssignSlotToAbility(*GASpec, SlotTag);

            MarkAbilitySpecDirty(*GASpec);

            ClientEquipAbility(AbilityTag, SlotTag, PrevSlot);
        }

    }
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
    const FGameplayTag& TargetSlotTag,
    const FGameplayTag& PrevSlotTag)
{
    AbilityEquippedDel.Broadcast(AbilityTag, FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped, TargetSlotTag, PrevSlotTag);

}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
    ActivatePassiveEffectDel.Broadcast(AbilityTag, bActivate);
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& Description, FString& NextLevelDescription)
{
    Description = FString("");
    NextLevelDescription = FString("");

    //if(!GetAvatarActor()->HasAuthority())return false;
    if (const auto& GASpec = GetSpecFromAbilityTag(AbilityTag))
    {
        if (auto AuraAbility = Cast<UAuraGameplayAbility>(GASpec->Ability))
        {
            Description = AuraAbility->GetDescription(this, AbilityTag, GASpec->Level);
            NextLevelDescription = AuraAbility->GetDescription(this, AbilityTag, GASpec->Level + 1);
            return true;
        }

    }

    if (auto AbilityInfo = UAuraAbilitySystemBPLibary::GetAbilityInfo(GetAvatarActor()))
    {
        Description = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
    }

    return false;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
    for (const auto& Tag : Spec->GetDynamicSpecSourceTags())
    {
        if (Slot.MatchesTagExact(Tag))
        {
            return true;
        }
    }

    return false;
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(FGameplayAbilitySpec* Spec)
{

    return Spec->GetDynamicSpecSourceTags().HasTag(FAuraGmaeplayTags::GetInstance().InputTag);
}

void UAuraAbilitySystemComponent::ClearAndAssignSlotToAbility(FGameplayAbilitySpec& GASpec, const FGameplayTag& NewSlot)
{
    ClearSlotAndChangeStatus(&GASpec, FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped);
    GASpec.GetDynamicSpecSourceTags().AddTag(NewSlot);
}

void UAuraAbilitySystemComponent::ClearSlotAndChangeStatus(FGameplayAbilitySpec* GASpec, const FGameplayTag& Status)
{
    if (GASpec)
    {
        const auto Slot = GetInputTagFromSpec(*GASpec);
        GASpec->GetDynamicSpecSourceTags().RemoveTag(Slot);
        GASpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusFromAbilityTag(GetAbilityTagFromSpec(*GASpec)));
        GASpec->GetDynamicSpecSourceTags().AddTag(Status);
    }
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
    FScopedAbilityListLock Locked(*this);

    for (auto& Spec : GetActivatableAbilities())
    {
        if (const bool bHasSlot = AbilityHasSlot(&Spec, SlotTag))
        {
            ClearSlotAndChangeStatus(&Spec);
        }
    }
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
    if (auto GASpec = GetSpecFromAbilityTag(AbilityTag))
    {
        auto GAStatus = GetStatusFromSpec(*GASpec);

        //升级或者解锁
        if (GAStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Eligible)) //可以学习
        {
            //技能只允许有一个状态标签 先删再加
            GASpec->GetDynamicSpecSourceTags().RemoveTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Eligible);
            GASpec->GetDynamicSpecSourceTags().AddTag(FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked);

            GAStatus = FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked;
        }
        else if (GAStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Unlocked)
            || GAStatus.MatchesTagExact(FAuraGmaeplayTags::GetInstance().Abilities_Status_Equipped)) //可升级
        {
            GASpec->Level += 1;
        }

        //扣点
        if (GetAvatarActor()->Implements<UPlayerInterface>())
        {
            IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
        }

        ClientUpdateAbilityStatus(AbilityTag, GAStatus, GASpec->Level);
        MarkAbilitySpecDirty(*GASpec);
    }

}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
    /**
     * 向监听事件的GA发送Tag事件，以达到升级的目的 (GA_ListenForEvent)
     * 该GA应用的GE添加各种Magnitude 并且设置标签 SetByCall 因此能通过标签调用对应的Magnitude 修改对应的值
     */

    FGameplayEventData Payload;
    Payload.EventTag = AttributeTag;
    Payload.EventMagnitude = 1.F;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

    //升级之后得减一点
    if (GetAvatarActor()->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
    }
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& GATag,
    const FGameplayTag& StatusTag,
    int32 GALevel)
{
    AbilityStatusChangeDel.Broadcast(GATag, StatusTag, GALevel);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    if (!bStartupAbilitiesGiven)
    {
        bStartupAbilitiesGiven = true;
        AbilitiesGiveDel.Broadcast(); //客户端
    }

}
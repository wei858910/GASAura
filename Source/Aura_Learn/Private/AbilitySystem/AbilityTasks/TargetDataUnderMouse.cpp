// 学习使用


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include <memory>

#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
    auto MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
    return MyObj;
}

void UTargetDataUnderMouse::Activate()
{

    const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled(); //是否为客户端本地
    if (bIsLocallyControlled)
    {
        SendMouseCursorData(); //向服务端发数据包装
    }
    else
    {
        //在服务器上监听 TargetData
        const auto SpecHandle = GetAbilitySpecHandle();
        const auto PredictionKey = GetActivationPredictionKey();
        AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
                              .AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

        const bool bCalledDel = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey); //若已经广播代理 则返回真
        if (!bCalledDel)
        {
            SetWaitingOnRemotePlayerData(); //等待数据
        }
    }

}

void UTargetDataUnderMouse::SendMouseCursorData()
{
    /**
     *  FScopedPredictionWindow 用于处理网络预测窗口的范围。这种类通常用于网络同步环境中
     *  主要作用是确保特定代码段在预测窗口内执行，以便在网络环境下正确处理动作的同步和预测。
     *	预测的变更 和 预测密钥相关 
     *	预测期 为其生命周期
     */
    FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

    auto Interface = Cast<ICombatInterface>(GetAvatarActor());
    if (!Interface)
    {
        return;
    }
    auto HitRes = Interface->GetCursorHitRes();
    if (!HitRes)
    {
        return;
    }

    //HitResult 内容的TargetData
    auto Data = new FGameplayAbilityTargetData_SingleTargetHit();

    FGameplayAbilityTargetDataHandle TargetDataHandle;
    Data->HitResult = *HitRes;
    TargetDataHandle.Add(Data); //装载数据

    AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
        GetActivationPredictionKey()/*原始预测密钥 与GA激活相关*/, TargetDataHandle,
        FGameplayTag{}, AbilitySystemComponent->ScopedPredictionKey/*当前密钥，GAS生成的，为该时的特定情景*/);

    if (ShouldBroadcastAbilityTaskDelegates()) //广播前判断GA仍然有效
    {
        ValidData.Broadcast(TargetDataHandle);
    }
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
    FGameplayTag ActivationTag)
{
    AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); //从客户端清除  TargetData

    if (ShouldBroadcastAbilityTaskDelegates()) //广播前判断GA仍然有效
    {
        ValidData.Broadcast(DataHandle);
    }
}
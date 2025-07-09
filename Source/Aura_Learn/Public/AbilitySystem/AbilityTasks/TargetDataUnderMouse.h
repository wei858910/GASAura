// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 该类是Task 类型 按照固定套路填充类 static函数会成蓝图节点   声明的代理会成为执行节点分支
 *
 * 用以执行GA时获取相应数据
 * 用以网络同步 防止激活GA时数据未达到客户端，客户端先执行了GA，导致无效执行
 * 通过服务端的广播进行通知，让数据到达之后 再继续执行
 */
UCLASS()
class AURA_LEARN_API UTargetDataUnderMouse : public UAbilityTask
{
    GENERATED_BODY()

public:
    //DefaultToSelf 表示默认参数为蓝图中的self，也就是调用者了  //BlueprintInternalUseOnly 意味着该函数通常不应该被蓝图用户直接调用
    UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName="TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
    static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility); //创建一个该类型对象

    UPROPERTY(BlueprintAssignable)
    FMouseTargetDataSignature ValidData;

private:
    virtual void Activate() override;

    //如果GA先促发，后才收到信息，GA调用为非预期
    void SendMouseCursorData(); //向服务器发送鼠标命中信息，然后在在其回调中触发GA

    void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
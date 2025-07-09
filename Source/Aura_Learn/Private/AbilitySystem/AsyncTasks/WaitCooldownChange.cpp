// 学习使用

#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "Algo/MaxElement.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,
    const FGameplayTag& InCooldonwTag)
{
    if (!IsValid(InAbilitySystemComponent) || !InCooldonwTag.IsValid())
    {
        return nullptr;
    }

    UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();

    WaitCooldownChange->ASC = InAbilitySystemComponent;
    WaitCooldownChange->CooldonwTag = InCooldonwTag;

    //注册Tag增减时的回调函数 模式为新增或者移除时 当冷却标签增减时调用
    InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldonwTag, EGameplayTagEventType::NewOrRemoved)
                            .AddUObject(WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged);

    //绑定 每当添加持续时间的GE，客户端和服务器都会调用该代理  用来知道冷却GE何时应用的
    InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

    return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
    if (!IsValid(ASC))
    {
        return;
    }

    //从该多播委托的调用列表中删除绑定到指定UserObject的所有函数
    ASC->RegisterGameplayTagEvent(CooldonwTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

    SetReadyToDestroy(); //标记对象已经准备好被销毁
    MarkAsGarbage();     //将对象标记为垃圾对象，让引擎知道对象不再需要，可在下次垃圾回收时进行处理
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
    if (NewCount == 0) //监听的冷却标签数量为0 则说明冷却完成了
    {
        CooldownEndDel.Broadcast(0.f);
    }
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec,
    FActiveGameplayEffectHandle GEHandle)
{
    //通过Tag匹配到对应的资源
    FGameplayTagContainer AssetTags;
    GESpec.GetAllAssetTags(AssetTags);

    FGameplayTagContainer GrantedTags;
    GESpec.GetAllGrantedTags(GrantedTags);

    if (AssetTags.HasTagExact(CooldonwTag) || GrantedTags.HasTagExact(CooldonwTag))
    {
        FGameplayEffectQuery CdGeQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldonwTag.GetSingleTagContainer()); //根据tag 创建GE的查询器
        auto CDArray = TargetASC->GetActiveEffectsTimeRemaining(CdGeQuery);

        //获取到CD
        if (CDArray.Num() > 0)
        {
            float MaxCD = *(Algo::MaxElement(CDArray)); //为了防止某些情况下，多个冷却GE的存在，取最大值
            CooldownStartDel.Broadcast(MaxCD);
        }

    }
}
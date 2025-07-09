/**
 * 蓝图异步节点，用以GA提交冷却时 检测对应的冷却Tag获取CD
 */
#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeDel, float, TimeRemaining); //冷却的代理

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask")) //暴露该任务对象到节点
class AURA_LEARN_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FCooldownChangeDel CooldownStartDel; //进入冷却

    UPROPERTY(BlueprintAssignable)
    FCooldownChangeDel CooldownEndDel; //结束冷却

    /*BlueprintInternalUseOnly="true" 可以在引擎内部或者框架的内部逻辑中使用，但它不会在标准蓝图编辑器的节点面板中显示，故蓝图用户无法直接调用或使用这个函数。*/
    UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
    static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldonwTag);

    UFUNCTION(BlueprintCallable)
    void EndTask();

protected:
    //该函数绑定到GAS的标签变化时调用
    void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
    void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle);
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayTag CooldonwTag;
};
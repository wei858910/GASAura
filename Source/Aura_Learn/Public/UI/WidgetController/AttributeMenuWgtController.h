// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWgtController.generated.h"

struct FGameplayTag;
class UAttributeInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSingature, const FAuraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class AURA_LEARN_API UAttributeMenuWgtController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    virtual void BindCallbackToDependencies() override;
    virtual void BroadcastInitialValues() override;

    void BroadcastAttributeInfo(const FGameplayTag& Tag) const; //从Tag 找到 属性 广播其值

    UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
    FAttributeInfoSingature OnAttributeInfoChangeDel;

    UPROPERTY(BlueprintAssignable, Category = "State|Attributes")
    FOnPlayerStateChangedSignature AttributePointsChangedDelegate;

protected:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAttributeInfo> AttributeInfo; //Data Asset 类

    UFUNCTION(BlueprintCallable)
    void UpgradeAttribute(const FGameplayTag& AttributeTag);
};
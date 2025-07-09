// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
struct FGameplayAttribute;
class UAuraUserWidget;
struct FOnAttributeChangeData;

USTRUCT()
struct FUIWidgetRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag MessageTag{};
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Message{};
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UAuraUserWidget> MessageWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Image{ nullptr };
};

/* 有属性存在变化时的代理*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, float, NewValue, bool, bShowMsg);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, RowInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_LEARN_API UOverlayWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallbackToDependencies() override;

protected:
    template <typename T>
    T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

    void BindAttributeChangeHelperFunc(FGameplayAttribute Attribute, FOnAttributeChangedSignature& AttributeData);

    void OnXPChanged(int32 NewXP);

    void OnAbilityEquipped(const FGameplayTag& GATag, const FGameplayTag& StatusTag, const FGameplayTag& TargetSlot, const FGameplayTag& PrevSlot); //技能装配
public:
    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes") //BlueprintAssignable用于多播代理在蓝图中分配
    FOnAttributeChangedSignature OnHealthChangedDel;
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxHealthChangedDel;
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnManaChangedDel;
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxManaChangedDel;
    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FMessageWidgetRowSignature MessageWidgetRowDel;

    UPROPERTY(BlueprintAssignable, Category = "State|XP")
    FOnAttributeChangedSignature OnXPPercentChangedDel; //经验有变动，经验条也该变动

    UPROPERTY(BlueprintAssignable, Category = "State|Level")
    FOnLevelChangedSignature OnPlayerLevelChangedDel;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetData") //EditDefaultsOnly 仅在类编辑器中可设置 实例化对象编辑器不可设置
    TObjectPtr<UDataTable> MessageWidgetDataTable;

};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) //通过标签获取行数据
{
    return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
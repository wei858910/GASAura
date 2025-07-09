#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
class UAuraAttributeSet;
class APlayerState;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, int32, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoDel, const FAuraAbilityInfo&, Info);

//用结构体来储存所有控制层将要交互的对象类型
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()
    FWidgetControllerParams() {};

    FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
        : PlayerController(PC)
        , PlayerState(PS)
        , AbilitySystemComponent(ASC)
        , AttributeSet(AS) {};
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController{ nullptr };
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerState> PlayerState{ nullptr };
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent{ nullptr };
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet{ nullptr };
};

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UAuraWidgetController : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

    UFUNCTION(BlueprintCallable)
    virtual void BroadcastInitialValues();     //通过调用各种代理通知 发送初始化的属性值 绑定代理与函数
    virtual void BindCallbackToDependencies(); //绑定GAS组件的代理
    void BroadcastAbilityInfo();               //当初始技能赋予完成时广播技能信息，由ASC中的AbilitiesGiveDel驱动

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FAbilityInfoDel AbilityInfoDel; //广播技能信息

protected:
    AAuraPlayerController* GetAuraPC();
    AAuraPlayerState* GetAuraPS();
    UAuraAbilitySystemComponent* GetAuraASC();
    UAuraAttributeSet* GetAuraAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category="WidgetController")
    TObjectPtr<APlayerController> PlayerController{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<AAuraPlayerController> AuraPlayerController{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<AAuraPlayerState> AuraPlayerState{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent{ nullptr };

    UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
    TObjectPtr<UAuraAttributeSet> AuraAttributeSet{ nullptr };


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
    TObjectPtr<UAbilityInfo> AbilityInfo; //技能信息表记得在蓝图中配

};
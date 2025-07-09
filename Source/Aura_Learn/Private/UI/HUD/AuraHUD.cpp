﻿// 学习使用


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AttributeMenuWgtController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/WidgetController/SpellMenuWgtController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayController(const FWidgetControllerParams& WCParams = FWidgetControllerParams())
{
    if (!IsValid(InstanceController))
    {
        InstanceController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
        InstanceController->SetWidgetControllerParams(WCParams);
        InstanceController->BindCallbackToDependencies(); //绑定GAS属性变化的代理到将被通知的函数
    }

    return InstanceController;
}

UAttributeMenuWgtController* AAuraHUD::GetAttributeMenuController(const FWidgetControllerParams& WCParams)
{
    if (!IsValid(InstanceAttributeController))
    {
        InstanceAttributeController = NewObject<UAttributeMenuWgtController>(this, AttributeMenuWgtControllerClass);
        InstanceAttributeController->SetWidgetControllerParams(WCParams);
        InstanceAttributeController->BindCallbackToDependencies();
    }
    return InstanceAttributeController;
}

USpellMenuWgtController* AAuraHUD::GetSpellMenuController(const FWidgetControllerParams& WCParams)
{
    if (!IsValid(InstanceSpellMenuController))
    {
        InstanceSpellMenuController = NewObject<USpellMenuWgtController>(this, SpellMenuWgtControllerClass);
        InstanceSpellMenuController->SetWidgetControllerParams(WCParams);
        InstanceSpellMenuController->BindCallbackToDependencies();
    }
    return InstanceSpellMenuController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass 没有选择值，请在AAuraHUD的派生类中检查是否有未设置")) //该check会将崩溃记录到日志
    checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass 没有选择值，请在AAuraHUD的派生类中检查是否有未设置"))

    OverlayWidget = Cast<UAuraUserWidget>(CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass));
    FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
    auto OverlayWidgetController = GetOverlayController(WidgetControllerParams); //初始化一下控制层实例
    OverlayWidget->SetWidgetController(OverlayWidgetController);                 //设置该widget的控制层

    OverlayWidgetController->BroadcastInitialValues(); //将个各个属性初始化后的当前值发送
    OverlayWidget->AddToViewport();
}
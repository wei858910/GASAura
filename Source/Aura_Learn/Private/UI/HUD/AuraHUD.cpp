// 学习使用


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlaiController(const FWidgetControllerParams& WCParams = FWidgetControllerParams())
{
	static TObjectPtr<UOverlayWidgetController>InstanceController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
	static bool bIsFirst{ true };//因为首次得设置WCParams 所以还得来个变量
	if(bIsFirst)
	{
		InstanceController->SetWidgetControllerParams(WCParams);
		bIsFirst = false;
	}

	return InstanceController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass 没有选择值，请在AAuraHUD的派生类中检查是否有未设置")) //该check会将崩溃记录到日志
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass 没有选择值，请在AAuraHUD的派生类中检查是否有未设置"))

	OverlayWidget = Cast<UAuraUserWidget>(CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass));
	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	auto OverlayWidgetController = GetOverlaiController(WidgetControllerParams);//初始化一下控制层实例
	OverlayWidget->SetWidgetController(OverlayWidgetController);//设置该widget的控制层
	OverlayWidget->AddToViewport();
}

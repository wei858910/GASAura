// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UAuraUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetController(UObject* InWidgetController);

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UObject> WidgetController;

protected:
    UFUNCTION(BlueprintImplementableEvent) //函数能在蓝图中实现
    void OnWidgetControllerSetEnd();       //设置控制层完成之后触发
};
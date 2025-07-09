// 学习使用


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
    WidgetController = InWidgetController;
    OnWidgetControllerSetEnd(); //通知蓝图事件
}
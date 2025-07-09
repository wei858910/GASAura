// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class USpellMenuWgtController;
class UAttributeMenuWgtController;
class APlayerState;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_LEARN_API AAuraHUD : public AHUD
{
    GENERATED_BODY()

public:
    UOverlayWidgetController* GetOverlayController(const FWidgetControllerParams& WCParams); //获取控制层的单例
    UAttributeMenuWgtController* GetAttributeMenuController(const FWidgetControllerParams& WCParams);
    USpellMenuWgtController* GetSpellMenuController(const FWidgetControllerParams& WCParams);

    //初始化控制层实例，将widget的控制层设置为初始化的实例，并且将其输出到屏幕
    void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> InstanceController{ nullptr };

    UPROPERTY(EditAnywhere)
    TSubclassOf<UAttributeMenuWgtController> AttributeMenuWgtControllerClass;
    UPROPERTY()
    TObjectPtr<UAttributeMenuWgtController> InstanceAttributeController{ nullptr };


    UPROPERTY(EditAnywhere)
    TSubclassOf<USpellMenuWgtController> SpellMenuWgtControllerClass;
    UPROPERTY()
    TObjectPtr<USpellMenuWgtController> InstanceSpellMenuController{ nullptr };

};
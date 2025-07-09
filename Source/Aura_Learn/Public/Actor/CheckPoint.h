#pragma once

#include "CoreMinimal.h"
#include "Aura_Learn/Aura_Learn.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 存档点 以及 其他 需要发光的Actor
 */
UCLASS()
class AURA_LEARN_API ACheckPoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
    GENERATED_BODY()

public:
    ACheckPoint(const FObjectInitializer& ObjectInitializer);

    /* HighlightInterface */
    virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
    virtual void HighlightActor_Implementation() override;
    virtual void UnHightlightActor_Implementation() override;
    /* HighlightInterface */

    /* SaveInterface */
    virtual bool ShouldLoadTransform_Implementation() override { return false; };
    virtual void LoadActor_Implementation() override; //如果已经存过裆，则让其亮起
    /* SaveInterface */
    UPROPERTY(SaveGame, BlueprintReadWrite) //Savegame能支持序列化
    bool bReached{ false };                 //存档过了？

    UPROPERTY(EditAnywhere, Category="是否绑定重叠的存档回调")
    bool bBindOverlapCallback{ true }; //是否调用球形重叠的回调
protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent)
    void CheckPointReached(UMaterialInstanceDynamic* DnamicMI); //BP中处理发光

    UFUNCTION(BlueprintCallable)
    void HandleGlowEffect(); //处理发光

    UPROPERTY(EditAnywhere, DisplayName="交互目标位置", meta=(MakeEditWidget="true"))
    TObjectPtr<class USceneComponent> TargetLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "存档点模型")
    TObjectPtr<UStaticMeshComponent> CheckPointMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "描边颜色")
    int32 CustomDepthStencilOverride{ Aura::CUSTOM_DEPTH_TAN };

private:
    UPROPERTY(VisibleAnywhere, DisplayName = "存档点重叠球")
    TObjectPtr<USphereComponent> Sphere;

    UPROPERTY()
    UMaterialInstanceDynamic* CachedDynamicMI; // 缓存的动态材质实例

};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_LEARN_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DnamicMI);//BP中处理发光

	void HandleGlowEffect();//处理发光

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName = "存档点模型")
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
private:

	UPROPERTY(VisibleAnywhere, DisplayName = "存档点重叠球")
	TObjectPtr<USphereComponent> Sphere;

	UMaterialInstanceDynamic* CachedDynamicMI; // 缓存的动态材质实例

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_LEARN_API ACheckPoint : public APlayerStart,public ISaveInterface
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/* SaveInterface */
	virtual bool ShouldLoadTransform_Implementation() override{return false;};
	virtual void LoadActor_Implementation() override;//如果已经存过裆，则让其亮起
	/* SaveInterface */
	UPROPERTY(SaveGame,BluePrintReadOnly)//Savegame能支持序列化
	bool bReached{ false };//存档过了？
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

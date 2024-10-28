#pragma once

#include "CoreMinimal.h"
#include "Actor/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()

public:
	/* HighlightInterface */
	virtual void HighlightActor_Implementation() override;
	/* HighlightInterface */

	/* SaveInterface */
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */

	UPROPERTY(EditAnywhere,DisplayName="目标关卡")
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, DisplayName = "目标关卡起点Tag")
	FName TargetMapStartTag;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

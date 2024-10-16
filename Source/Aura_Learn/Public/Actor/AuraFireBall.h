#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * ¼Ì³Ð×Ô·ÉÉäÎï£¬»ðÇò»Ø¹ö
 */
UCLASS()
class AURA_LEARN_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LaunchFire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BackToOnwer();

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

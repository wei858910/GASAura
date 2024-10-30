#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_LEARN_API AAuraEnemySpawnVolume : public AActor,public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();

	/* SaveInterface */
	virtual void LoadActor_Implementation() override;
	/* SaveInterface */

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	UPROPERTY(BlueprintReadOnly,SaveGame)
	bool bReached{ false };//已经到达过


	UPROPERTY(EditAnywhere,DisplayName="敌人生成集")
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

private:
	UPROPERTY(VisibleAnywhere,DisplayName="触发盒")
	TObjectPtr<UBoxComponent> OverlapBox;

};

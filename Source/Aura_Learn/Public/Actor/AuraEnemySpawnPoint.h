#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_LEARN_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere,Category="生成",DisplayName="生成的对象类")
	TSubclassOf<AAuraEnemy> SpawnEnemyClass;

	UPROPERTY(EditAnywhere, Category = "生成", DisplayName = "对象等级")
	int32 EnemyLevel{ 1 };

	UPROPERTY(EditAnywhere, Category = "生成", DisplayName = "对象职业")
	ECharacterClass  EnemyClass{ ECharacterClass ::Warrior};//职业
};

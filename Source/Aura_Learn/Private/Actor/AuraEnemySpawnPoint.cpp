#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; //调整位置生成

    auto AuraEnemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(SpawnEnemyClass, GetActorTransform());
    AuraEnemy->SetEnemyLevel(EnemyLevel);
    AuraEnemy->setCharacterClass(EnemyClass);
    AuraEnemy->FinishSpawning(GetActorTransform());
    AuraEnemy->SpawnDefaultController();
}
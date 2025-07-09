#include "Actor/AuraEnemySpawnVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    OverlapBox = CreateDefaultSubobject<UBoxComponent>("Box");
    SetRootComponent(OverlapBox);
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapBox->SetCollisionObjectType(ECC_WorldStatic);
    OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
    if (bReached)
    {
        Destroy();
    }
}

// Called when the game starts or when spawned
void AAuraEnemySpawnVolume::BeginPlay()
{
    Super::BeginPlay();

    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
}

void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (!OtherActor->Implements<UPlayerInterface>())
    {
        return;
    }

    bReached = true;
    for (AAuraEnemySpawnPoint* Point : SpawnPoints)
    {
        if (IsValid(Point))
        {
            Point->SpawnEnemy();
        }
    }
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
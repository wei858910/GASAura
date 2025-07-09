#include "Actor/MapEntrance.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AMapEntrance::HighlightActor_Implementation()
{
    CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation() {}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor->Implements<UPlayerInterface>())
    {
        bReached = true;
        if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
        {
            AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName(), TargetMapStartTag);
        }

        IPlayerInterface::Execute_SaveProgress(OtherActor, TargetMapStartTag);

        UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
    }
}
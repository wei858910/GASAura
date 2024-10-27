#include "Actor/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	CheckPointMesh->SetupAttachment(Sphere);
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckPointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckPointMesh->MarkRenderStateDirty();//标记渲染状态为“脏”状态,通知引擎该渲染状态发生变化，需更新渲染
}

void ACheckPoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination= MoveToLocation+GetActorLocation();
}

void ACheckPoint::HighlightActor_Implementation()
{
	if(!bReached)
	{
		CheckPointMesh->SetRenderCustomDepth(true);
	}
}

void ACheckPoint::UnHightlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::LoadActor_Implementation()
{
	if(bReached)
	{
		HandleGlowEffect();
	}
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	if(bBindOverlapCallback)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);
	}
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld());
		}


		IPlayerInterface::Execute_SaveProgress(OtherActor,PlayerStartTag	);
		HandleGlowEffect();
	}
}

void ACheckPoint::HandleGlowEffect()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!CachedDynamicMI)
	{
		CachedDynamicMI = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0), this);
		CheckPointMesh->SetMaterial(0, CachedDynamicMI);
	}

	CheckPointReached(CachedDynamicMI);
}

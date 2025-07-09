// 学习使用

#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const FName TargetTag = AIOwner->GetPawn()->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

    TArray<AActor*> FoundActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(AIOwner->GetPawn(), TargetTag, FoundActorsWithTag);

    float ClosestDistance = TNumericLimits<float>::Max();
    AActor* ClosestActor = nullptr;
    for (AActor* Actor : FoundActorsWithTag)
    {

        if (IsValid(Actor) && IsValid(AIOwner->GetPawn()))
        {
            const float Distance = AIOwner->GetPawn()->GetDistanceTo(Actor);
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestActor = Actor;
            }
        }
    }
    UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
    UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
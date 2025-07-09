// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
    GENERATED_BODY()

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; //行为树服务，按照设定的频率定期执行非每帧执行

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector TargetToFollowSelector; //跟踪的目标黑板键选择器

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector DistanceToTargetSelector; //距离目标距离的黑板键选择器
};
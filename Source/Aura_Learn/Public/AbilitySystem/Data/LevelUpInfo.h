//管理升级和经验相关的资料
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 经验值累加，升级后不清零
 */
USTRUCT(BlueprintType)
struct FLevelInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, DisplayName = "等级")
    int16 Level{ 0 };
    UPROPERTY(EditDefaultsOnly, DisplayName="升级所需经验")
    int32 LevelUpRequirement{ 0 };

    UPROPERTY(EditDefaultsOnly, DisplayName = "属性奖励点")
    int16 AttributePointAward{ 1 };

    UPROPERTY(EditDefaultsOnly, DisplayName = "法术奖励点")
    int16 SpellPointAward{ 1 };
};

UCLASS()
class AURA_LEARN_API ULevelUpInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    int16 FindLevelForXP(const int32 InXP); //从给定的经验值获取当前的等级

    /**
     * 等级已经让函数自动添加  数组的索引 = Level-1
     */
    UPROPERTY(EditDefaultsOnly, DisplayName = "等级的信息组", Category="升级信息")
    TArray<FLevelInfo> LevelUpInformation;

protected:
    #if WITH_EDITOR
    // 当在编辑器中更改属性时调用 这里主要想让等级自动填入
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    #endif
};
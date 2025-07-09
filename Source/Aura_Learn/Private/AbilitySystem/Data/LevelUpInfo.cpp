// 学习使用


#include "AbilitySystem/Data/LevelUpInfo.h"

int16 ULevelUpInfo::FindLevelForXP(const int32 InXP)
{
    int16 Level{ 1 };
    for (const auto Info : LevelUpInformation)
    {
        if (InXP >= Info.LevelUpRequirement)
        {
            Level = Info.Level + 1; //查询经验都大于升级所需经验了，那肯定到达下一级了
        }
        else
        {
            return Level;
        }
    }
    return Level;
}

#if WITH_EDITOR
void ULevelUpInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 遍历数组，更新每个 FLevelInfo 的 Level 属性
    for (int32 Index = 0; Index < LevelUpInformation.Num(); ++Index)
    {
        LevelUpInformation[Index].Level = Index + 1;
    }
}
#endif
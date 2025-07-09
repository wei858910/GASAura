// 学习使用
#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpwanLocation()
{
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

    const float DeltaSpread = SpawnSpread / NumMinions; //计算每个生成物应该占用多少度的生成区域

    //扇形区域两边，由两个向量组成
    const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.F, FVector::UpVector); //绕向量旋转一定角度得到的向量
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.F, FVector::UpVector);

    TArray<FVector> SpawnLocations;
    for (int16 i{ 0 }; i < NumMinions; ++i)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);                //生成点位的向量
        FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance); //生成位置

        //检测 该位置的地面上投影坐标
        FHitResult HitRes;
        GetWorld()->LineTraceSingleByChannel(HitRes, ChosenSpawnLocation + FVector(0, 0, 300), ChosenSpawnLocation + FVector(0, 0, -300), ECC_Visibility);
        if (HitRes.bBlockingHit)
        {
            ChosenSpawnLocation = HitRes.ImpactPoint;
        }
        SpawnLocations.Emplace(ChosenSpawnLocation);
    }

    return SpawnLocations;
}

TSubclassOf<AActor> UAuraSummonAbility::GetRandomMinionClass()
{
    const int16 SelectionIndex = FMath::RandRange(0, MinionClasses.Num() - 1);
    if (IsValid(MinionClasses[SelectionIndex]))
    {
        return MinionClasses[SelectionIndex];
    }
    return nullptr;
}
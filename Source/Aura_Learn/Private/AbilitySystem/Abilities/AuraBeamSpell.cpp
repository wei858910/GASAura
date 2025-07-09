// 学习使用

#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        MouseHitLocation = HitResult.ImpactPoint;
        MouseHitActor = HitResult.GetActor();
    }
    else
    {
        CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
    }
}

void UAuraBeamSpell::StoreOnwerVaribles()
{
    if (CurrentActorInfo)
    {
        OnwerPlayerController = CurrentActorInfo->PlayerController.Get();
        OnwerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
    }

}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
    check(OnwerCharacter)
    if (!OnwerCharacter->Implements<UCombatInterface>())
    {
        return;
    }
    if (auto WeapnCmpt = ICombatInterface::Execute_GetWeapon(OnwerCharacter))
    {
        TArray<AActor*> IgnorelActor;
        IgnorelActor.Add(OnwerCharacter);
        auto Begin = WeapnCmpt->GetSocketLocation(FName("TipSocket"));
        FHitResult HitRes;

        /*ETraceTypeQuery::TraceTypeQuery1 是可视性通道*/
        UKismetSystemLibrary::SphereTraceSingle(OnwerCharacter, Begin, BeamTargetLocation, 20.f,
            TraceTypeQuery1, false, IgnorelActor,
            EDrawDebugTrace::None, HitRes, true);

        if (HitRes.bBlockingHit)
        {
            MouseHitLocation = HitRes.ImpactPoint;
            MouseHitActor = HitRes.GetActor();
        }
    }

    if (auto CombatIF = Cast<ICombatInterface>(MouseHitActor))
    {
        //防止重复绑定
        if (!CombatIF->GetOnDeathDel().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
        {
            CombatIF->GetOnDeathDel().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
        }

    }

}

/**
 * 闪电击中的首个敌人周围的捕获
 * 排除技能发出者 和 首个敌人
 */
void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
    TArray<AActor*> OverlappingActors;
    TArray<AActor*> ActorToIgnore;
    ActorToIgnore.Add(GetAvatarActorFromActorInfo());
    ActorToIgnore.Add(MouseHitActor);

    UAuraAbilitySystemBPLibary::GetLivePlayersWithRadius(
        GetAvatarActorFromActorInfo(),
        OverlappingActors, ActorToIgnore,
        MouseHitActor->GetActorLocation(),
        StartupAddtionalLen + GetAbilityLevel() * 50);

    int32 NumAdditionalTargets = FMath::Min(MaxNumShockTargets, GetAbilityLevel() - 1);

    UAuraAbilitySystemBPLibary::GetClosesTarget(MouseHitActor->GetActorLocation(), NumAdditionalTargets, OverlappingActors, OutAdditionalTargets);

    //死亡时移除特效的绑定
    for (auto Actor : OutAdditionalTargets)
    {
        if (auto CombatIF = Cast<ICombatInterface>(Actor))
        {
            //防止重复绑定
            if (!CombatIF->GetOnDeathDel().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
            {
                CombatIF->GetOnDeathDel().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
            }
        }
    }
}

void UAuraBeamSpell::RemoveOnDeathNotify(AActor* Actor)
{
    if (const auto CombatInterface = Cast<ICombatInterface>(Actor))
    {
        if (CombatInterface->GetOnDeathDel().IsAlreadyBound(this, &ThisClass::AdditionalTargetDied))
        {
            CombatInterface->GetOnDeathDel().RemoveDynamic(this, &ThisClass::AdditionalTargetDied);
        }
        if (CombatInterface->GetOnDeathDel().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied))
        {
            CombatInterface->GetOnDeathDel().RemoveDynamic(this, &ThisClass::PrimaryTargetDied);
        }
    }
}
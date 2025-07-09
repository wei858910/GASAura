// 学习使用

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
    PrimaryActorTick.bCanEverTick = false;
    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
    Super::BeginPlay();
    if (bFloattActor)
    {
        FloatctorEvent();
    }
}

void AAuraEffectActor::ApplyEffecToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GEClass, const FEffectWithPolicy& CurentEffectPolicy)
{
    if (!TargetActor->Implements<UCombatInterface>())
    {
        return;
    }
    if (TargetActor->ActorHasTag("Enemy") && !CurentEffectPolicy.bApplyToEnemy)
    {
        return; //敌人并且不允许敌人拾取直接返回
    }
    check(GEClass)
    auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!IsValid(TargetASC))
    {
        return;
    }

    auto EffectContext = TargetASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);
    auto EffectSpec = TargetASC->MakeOutgoingSpec(GEClass, ActorLevel, EffectContext);
    TargetASC->ApplyGameplayEffectSpecToSelf(*(EffectSpec.Data.Get()));

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
    bool bHasEnemyCanTake{ false };
    bool bHasInfinite{ false }; //GE中存在无限的
    for (auto& it : EffectWithPolicys)
    {
        if (it.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
        {
            ApplyEffecToTarget(TargetActor, it.GameplayEffectClass, it);
            if (IsValid(it.GameplayEffectClass))
            {
                auto DefaultGE = it.GameplayEffectClass->GetDefaultObject<UGameplayEffect>(); //获取该类默认实例
                if (DefaultGE->DurationPolicy == EGameplayEffectDurationType::Infinite)
                {
                    bHasInfinite = true;
                }
            }

            bHasEnemyCanTake |= it.bApplyToEnemy;
        }

    }

    //存在敌人能拾取，并且在应用GE之后销毁,并且所有GE中不存在无限效果 才销毁
    if ((!TargetActor->ActorHasTag("Enemy") || bHasEnemyCanTake) && bDestroyOnEffectApplication && !bHasInfinite)
    {
        Destroy();
    }
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
    auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!IsValid(TargetAsc))
    {
        return;
    }

    for (auto& it : EffectWithPolicys)
    {
        if (it.EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
        {
            ApplyEffecToTarget(TargetActor, it.GameplayEffectClass, it);
        }

        //移除无限GE
        if (it.InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndoverlap)
        {
            TargetAsc->RemoveActiveGameplayEffectBySourceEffect(it.GameplayEffectClass, TargetAsc, 1); //移除一层
        }
    }
}
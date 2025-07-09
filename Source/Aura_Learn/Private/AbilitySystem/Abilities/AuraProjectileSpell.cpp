#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitieDescriptions.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

FString UAuraProjectileSpell::GetDescription(const UAuraAbilitySystemComponent* AuraGAS, const FGameplayTag& GATag, const int32 Level)
{
    if (!AuraGAS)
    {
        return FString{};
    }

    const int32 Damage = GetDamageByDamageType(FAuraGmaeplayTags::GetInstance().Damage_Fire, Level);
    FText CostofMana = FText::FromString(FString::Printf(TEXT("%.1f"), GetManaCost(Level)));
    FText CoolDown = FText::FromString(FString::Printf(TEXT("%.1f"), GetCooldown(Level)));

    auto Description = FText::Format(AuraGAS->AbilityDescriptions->FindDescriptionsByTagAndLevel(GATag, Level), FMath::Min(Level, NumProjectiles), Damage, Level, CostofMana, CoolDown);
    return Description.ToString();
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, bool bOveridePitch, float PitchOverride)
{
    //单人游戏或者是在服务器
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
    if (!bIsServer)
    {
        return;
    }

    auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
    if (!CombatInterface)
    {
        return;
    }

    const auto SocketLocation = ICombatInterface::Execute_GetCombatSocktLocation
        (GetAvatarActorFromActorInfo(), FAuraGmaeplayTags::GetInstance().CombatSocket_Weapon);

    FRotator Rotation = (TargetLocation - SocketLocation).Rotation(); //获取两个位置的旋转差
    //如果想用新的pitch旋转
    if (bOveridePitch)
    {
        Rotation.Pitch = PitchOverride;
    }

    //Rotation += GetAvatarActorFromActorInfo()->GetActorRotation();
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(SocketLocation);
    SpawnTransform.SetRotation(Rotation.Quaternion());

    //延期生成 调用finish 完成生成
    auto ProjectfileActor = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectfileClass, SpawnTransform, GetOwningActorFromActorInfo(),
        Cast<APawn>(GetOwningActorFromActorInfo()),
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    ProjectfileActor->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

    ProjectfileActor->FinishSpawning(SpawnTransform);
}
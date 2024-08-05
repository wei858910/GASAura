// 学习使用

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false; 
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	//武器设置
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));//武器绑定到武器插槽
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);//关闭武器碰撞
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InitializeGEClass, float Level) const
{
	if (!IsValid(InitializeGEClass) || !IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("DefaultSecondaryAtributes Not Setup Form AAuraCharacterBase!!!"))
		return;
	}
	auto InitGESpec = AbilitySystemComponent->MakeEffectContext();
	InitGESpec.AddSourceObject(this);
	auto InitGEHandle = AbilitySystemComponent->MakeOutgoingSpec(InitializeGEClass, Level, InitGESpec);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*InitGEHandle.Data.Get());
}

void AAuraCharacterBase::InitDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAtributes);
	ApplyEffectToSelf(DefaultSecondaryAtributes);
	ApplyEffectToSelf(DefaultVitalAtributes);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	auto GAS = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	if ((!HasAuthority())||!IsValid(GAS))return; //Actor是否有网络权限（就是是否为服务端的实例）
	GAS->AddCharacterAbilities(StartupAbilities);
}

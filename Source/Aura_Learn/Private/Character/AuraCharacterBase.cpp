// 学习使用

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

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

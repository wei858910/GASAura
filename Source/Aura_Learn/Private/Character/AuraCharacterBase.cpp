// 学习使用

#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false; 
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	//武器设置
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));//武器绑定到武器插槽
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);//关闭武器碰撞
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecCalc_Damage.generated.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	};
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UCLASS()
class AURA_LEARN_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                            FGameplayEffectCustomExecutionOutput& OutExecutionOutput)const override;
};

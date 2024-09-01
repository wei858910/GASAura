// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	AuraDamageStatics();
	void InitTagToCaptureDef();

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>TagToCaptureDef;//抗性类型映射相应的属性获取
};

static const AuraDamageStatics& GetDamageStatics()
{
	static AuraDamageStatics DStatics;
	if(DStatics.TagToCaptureDef.Num()<2)
	{
		DStatics.InitTagToCaptureDef();
	}
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

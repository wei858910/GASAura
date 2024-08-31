// 学习使用
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "Interaction/CombatInterface.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	//获取ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	//获取目标和源Actor
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//获取挂载此类的GE实例
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//设置评估参数
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//通过caller获取伤害值
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGmaeplayTags::GetInstance().Damage);

	//将格挡机会参与计算
	float BlockChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
	BlockChance=FMath::Max(0.f, BlockChance);
	bool bBlock= BlockChance>FMath::RandRange(0, 100);
	Damage = bBlock ? Damage / 2 : Damage;

	//GE 设置上下文内容
	auto GEContext = Spec.GetContext();
	UAuraAbilitySystemBPLibary::SetIsBlockedHit(GEContext, bBlock);

	/**
	 *  护甲穿透将影响护甲的防护效果
	 */
	float TargetArmor{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	float SourceArmorPenetration{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	auto DefaultClassInfo=UAuraAbilitySystemBPLibary::GetCharacterClassInfo(SourceAvatar);

	//获取表格曲线从表格曲线读值 分别是目标护甲的系数 和 源穿甲的系数
	auto ArmorPenetrationCurve = DefaultClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());//获取对应曲线
	auto SourceInterface=Cast<ICombatInterface>(SourceAvatar);
	const float  ArmorPenetrationRatio = ArmorPenetrationCurve->Eval(SourceInterface->GetPlayerLevel());//从给定X读取Y

	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration* ArmorPenetrationRatio) / 100.f;

	auto ArmorCurve= DefaultClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectArmor"), FString());
	auto TargetInterface = Cast<ICombatInterface>(TargetAvatar);
	const float ArmorRatio = ArmorCurve->Eval(TargetInterface->GetPlayerLevel());

	Damage *= (100-EffectiveArmor* ArmorRatio) / 100.f;

	/**
	 *  暴击
	 */
	float CriticalHitChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, CriticalHitChance);
	CriticalHitChance = FMath::Max(0.f, CriticalHitChance);
	const bool bCriticalHit = CriticalHitChance > FMath::RandRange(0, 100);

	if(bCriticalHit)
	{
		UAuraAbilitySystemBPLibary::SetIsCriticalHit(GEContext, true);

		float CriticalHitDamage{ 0.f };
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
		CriticalHitDamage = FMath::Max(0.f, CriticalHitDamage);

		float CriticalHitResistance{ 0.f };
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, CriticalHitResistance);
		CriticalHitResistance = FMath::Max(0.f, CriticalHitResistance);

		float CriticalHitResistanceRatio = CriticalHitResistance/(100 + CriticalHitResistance);//对数增长形式的系数，最终只能无限接近1
		CriticalHitDamage = CriticalHitDamage - (CriticalHitDamage * CriticalHitResistanceRatio);
		Damage += CriticalHitDamage*2.f;
	}

	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

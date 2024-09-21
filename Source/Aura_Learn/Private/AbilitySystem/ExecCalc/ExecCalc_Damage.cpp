// 学习使用
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemBPLibary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

AuraDamageStatics::AuraDamageStatics()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)

}

void AuraDamageStatics::InitTagToCaptureDef()
{
	const auto& Tags = FAuraGmaeplayTags::GetInstance();
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_Armor, ArmorDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Resistance_Fire, FireResistanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Resistance_Lightning, LightningResistanceDef);
	TagToCaptureDef.Emplace(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);
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
	float Damage{0.f};

	for (const auto& it : FAuraGmaeplayTags::GetInstance().DamageTagToResistanceTag)
	{
		const auto& DamageTypeTag = it.Key;
		const auto& DamageResistanceTag = it.Value;

		if (!GetDamageStatics().TagToCaptureDef.Contains(DamageResistanceTag))
		{
			auto temp = GetDamageStatics();
			UE_LOG(LogTemp, Error, TEXT("TagToCaptureDef 不存在Tag: [%s]"), *DamageResistanceTag.ToString());
			continue;
		}
		//当前的抗性属性
		const auto& CaptureDef = GetDamageStatics().TagToCaptureDef[DamageResistanceTag];//直接拿到当前的属性捕获类型 下面就方便直接获取值
		if(float CurentTypeDamge = Spec.GetSetByCallerMagnitude(it.Key,false)) //从GE中找寻相应Tag的Modfiy 修饰符 获取其值,没找到不给警告
		{
			float CurResistance{ 0.f };
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, CurResistance);

			CurResistance = FMath::Clamp(CurResistance, 0.f, 100.f);
			CurentTypeDamge *= (100.f - CurResistance) / 100.f;
			Damage += CurentTypeDamge;
		}

	}

	//将格挡机会参与计算
	float BlockChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	float SourceArmorPenetration{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	auto DefaultClassInfo=UAuraAbilitySystemBPLibary::GetCharacterClassInfo(SourceAvatar);

	//获取表格曲线从表格曲线读值 分别是目标护甲的系数 和 源穿甲的系数
	auto ArmorPenetrationCurve = DefaultClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());//获取对应曲线
	auto SourceInterface=Cast<ICombatInterface>(SourceAvatar);
	const float  ArmorPenetrationRatio = ArmorPenetrationCurve->Eval(ICombatInterface::Execute_GetPlayerLevel(SourceAvatar));//从给定X读取Y

	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration* ArmorPenetrationRatio) / 100.f;

	auto ArmorCurve= DefaultClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectArmor"), FString());
	auto TargetInterface = Cast<ICombatInterface>(TargetAvatar);
	const float ArmorRatio = ArmorCurve->Eval(ICombatInterface::Execute_GetPlayerLevel(TargetAvatar));

	Damage *= (100-EffectiveArmor* ArmorRatio) / 100.f;

	/**
	 *  暴击
	 */
	float CriticalHitChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, CriticalHitChance);
	CriticalHitChance = FMath::Max(0.f, CriticalHitChance);
	const bool bCriticalHit = CriticalHitChance > FMath::RandRange(0, 100);

	if(bCriticalHit)
	{
		UAuraAbilitySystemBPLibary::SetIsCriticalHit(GEContext, true);

		float CriticalHitDamage{ 0.f };
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
		CriticalHitDamage = FMath::Max(0.f, CriticalHitDamage);

		float CriticalHitResistance{ 0.f };
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, CriticalHitResistance);
		CriticalHitResistance = FMath::Max(0.f, CriticalHitResistance);

		float CriticalHitResistanceRatio = CriticalHitResistance/(100 + CriticalHitResistance);//对数增长形式的系数，最终只能无限接近1
		CriticalHitDamage = CriticalHitDamage - (CriticalHitDamage * CriticalHitResistanceRatio);
		Damage += CriticalHitDamage*2.f;
	}

	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

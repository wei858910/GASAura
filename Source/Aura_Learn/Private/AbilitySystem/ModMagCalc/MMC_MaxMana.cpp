// 学习使用


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
    IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute(); //设置属性
    IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;    //其实都一样 因为自己给自己应用 src=tar
    IntelligenceDef.bSnapshot = false;
    RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    auto SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
    auto TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTag;
    EvaluateParameters.TargetTags = TargetTag;

    //从属性集捕捉定义(FGameplayEffectAttributeCaptureDefinition) 获取目标值
    float Intelligence{ 0.f };
    GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intelligence);
    Intelligence = FMath::Max(0, Intelligence); //防止小于0

    auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
    if (!CombatInterface)
    {
        return 0.f;
    }
    auto PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());

    return 40.f + 8.f * Intelligence + 5.f * PlayerLevel; //返回想要得到的结果
}
// 学习使用


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
    //添加要捕捉的属性及相关设置
    VigDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();     //设置属性
    VigDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; //其实都一样 因为自己给自己应用 src=tar
    VigDef.bSnapshot = false;
    RelevantAttributesToCapture.Add(VigDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    auto SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
    auto TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTag;
    EvaluateParameters.TargetTags = TargetTag;

    //从属性集捕捉定义(FGameplayEffectAttributeCaptureDefinition) 获取目标值
    float Vigor{ 0.f };
    GetCapturedAttributeMagnitude(VigDef, Spec, EvaluateParameters, Vigor);
    Vigor = FMath::Max(0, Vigor); //防止小于0

    auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
    if (!CombatInterface)
    {
        return 0.f;
    }
    auto PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());

    return 10.f + 2.5f * Vigor + 5.f * PlayerLevel; //返回想要得到的结果
}
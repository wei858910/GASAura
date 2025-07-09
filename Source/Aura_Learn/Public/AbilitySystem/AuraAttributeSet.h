// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AuraAttributeSet.generated.h"

#define \
ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//用以储存GAS 相关的组件内容
USTRUCT()
struct FEffectProperties
{
    GENERATED_BODY()

    FEffectProperties() {};

    FGameplayEffectContextHandle EffectContextHandle;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceASC{ nullptr };
    UPROPERTY()
    TObjectPtr<AActor> SourceAvatarActor{ nullptr };
    UPROPERTY()
    TObjectPtr<AController> SourceController{ nullptr };
    UPROPERTY()
    TObjectPtr<ACharacter> SourceCharacter{ nullptr };

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetASC{ nullptr };
    UPROPERTY()
    TObjectPtr<AActor> TargetAvatarActor{ nullptr };
    UPROPERTY()
    TObjectPtr<AController> TargetController{ nullptr };
    UPROPERTY()
    TObjectPtr<ACharacter> TargetCharacter{ nullptr };
};

UCLASS()
class AURA_LEARN_API UAuraAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UAuraAttributeSet();

    //指定哪些属性需要在服务器和客户端之间同步
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //Current Value被改变前调用
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; //Base Value改变后调用

    virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override; //属性已经修改后调用

private:
    void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
    //将拿到的数据应用上FEffectProperties中

    void ShowFloatingText(const FEffectProperties& Prop, float Damage, const bool bBlocked = false, const bool bCriticalHit = false);
    void SendXPEvent(const FEffectProperties& Prop); //想目标GAS投递Tag事件

    void HandleIncomingDamage(const FEffectProperties&); //处理伤害事件

    void HandleIncomingXP(const FEffectProperties& Props); //处理经验事件

    void Debuff(const FEffectProperties& Props);

public:
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

    UFUNCTION()
    void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

    UFUNCTION()
    void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

    UFUNCTION()
    void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

    UFUNCTION()
    void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

    UFUNCTION()
    void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

    UFUNCTION()
    void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

    UFUNCTION()
    void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

    UFUNCTION()
    void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

    UFUNCTION()
    void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

    UFUNCTION()
    void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

    UFUNCTION()
    void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

    UFUNCTION()
    void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

    UFUNCTION()
    void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

    UFUNCTION()
    void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

    /*
     * Primary 属性 RPG属性
     */

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
    FGameplayAttributeData Strength; //力量
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
    FGameplayAttributeData Intelligence; //智力
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
    FGameplayAttributeData Resilience; //韧性 抑或是恢复力
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
    FGameplayAttributeData Vigor; //活力
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
    /*
     *Vital 属性 基础角色属性
     */

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

    /*
     *  Secondary Attributes 次要属性
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor); //护甲

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
    FGameplayAttributeData ArmorPenetration; //穿甲
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
    FGameplayAttributeData BlockChance; //格挡几率
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitChance; //暴击几率
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitDamage; //暴击伤害
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
    FGameplayAttributeData CriticalHitResistance; //扛暴击率
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
    FGameplayAttributeData HealthRegeneration; //回血率
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
    FGameplayAttributeData ManaRegeneration; //回蓝率
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

    /**
     * 抗性属性
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
    FGameplayAttributeData FireResistance;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Resistance Attributes")
    FGameplayAttributeData LightningResistance;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Resistance Attributes")
    FGameplayAttributeData ArcaneResistance;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
    FGameplayAttributeData PhysicalResistance;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

    /*	*
     *	元属性
     */

    UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
    FGameplayAttributeData IncomingDamage; //即将到来的伤害
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

    UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
    FGameplayAttributeData IncomingXP; //即将到来的经验
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);

private:
    //升级时改变，用以判断升级后是否设置属性
    bool bTopOffHealth{ false }; //是否将要更新生命值为最大生命值
    bool bTopOffMana{ false };   //是否将要更新法力值为最大法力值

    // 计时器变量 为了防止短期的伤害 蜂拥导致飘字重叠
    FTimerHandle FloatingTextTimerHandle;

    // 累加伤害的变量
    float AccumulatedDamage = 0.f;

    // 伤害计时器的回调函数
    void OnShowFloatingText(FEffectProperties Props, bool bBlockedHit, bool bCriticalHit);
};
// 学习使用


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGmaeplayTags& FAuraGmaeplayTags::GetInstance()
{
	static FAuraGmaeplayTags Instance{};
	return Instance;
}

void FAuraGmaeplayTags::InitNaviveGameplayTags()
{
	//Primary Attributes
	GetInstance().InitPrimaryAttributeTags();

	//Secondary Attributes
	GetInstance().InitSecondaryAttributeTags();

	/*输入*/
	GetInstance().InitInputTags();

	/*效果*/
	GetInstance().InitEffectTags();

	GetInstance().InitDamageTypesTags();

	/*技能*/
	GetInstance().InitSkillTags();
}

void FAuraGmaeplayTags::InitPrimaryAttributeTags()
{
	GetInstance().Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("增加物理伤害")
	);

	GetInstance().Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("增加魔法伤害")
	);

	GetInstance().Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("增加护甲和护甲穿透")
	);

	GetInstance().Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("增强血量")
	);
}

void FAuraGmaeplayTags::InitSecondaryAttributeTags()
{

	GetInstance().Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("减少受到的伤害，提高格挡几率")
	);

	GetInstance().Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("无视敌方百分比护甲，增加暴击几率")
	);

	GetInstance().Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("有几率将受到的伤害减半")
	);

	GetInstance().Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("有几率使伤害加倍并获得重击加成")
	);

	GetInstance().Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
	);

	GetInstance().Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("命中要害时增加额外伤害")
	);

	GetInstance().Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("每 1 秒再生的健康值")
	);

	GetInstance().Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("每 1 秒再生的法力值")
	);

	GetInstance().Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("可获得的最大健康值")
	);

	GetInstance().Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("可获得的最大法力值")
	);
}

void FAuraGmaeplayTags::InitInputTags()
{
	GetInstance().InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("鼠标左键")
	);
	GetInstance().InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("鼠标右键")
	);
	GetInstance().InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("键盘1键")
	);
	GetInstance().InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("键盘2键")
	);
	GetInstance().InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("键盘3键")
	);
	GetInstance().InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("键盘4键")
	);
}

void FAuraGmaeplayTags::InitDamageTypesTags()
{
	GetInstance().Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("伤害标签")
	);

	/*伤害类型*/
	GetInstance().Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("火性伤害")
	);
	GetInstance().Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("带雷伤害")
	);
	GetInstance().Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("元素伤害")
	);
	GetInstance().Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("物理伤害")
	);


	/*伤害类型抗性*/
	GetInstance().Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("火性伤害抗性")
	);
	GetInstance().Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("带雷伤害抗性")
	);
	GetInstance().Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("元素伤害抗性")
	);
	GetInstance().Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("物理伤害抗性")
	);

	/*伤害类型映射相应抵抗*/
	GetInstance().DamageTagToResistanceTag.Emplace(GetInstance().Damage_Fire, GetInstance().Attributes_Resistance_Fire);
	GetInstance().DamageTagToResistanceTag.Emplace(GetInstance().Damage_Lightning, GetInstance().Attributes_Resistance_Lightning);
	GetInstance().DamageTagToResistanceTag.Emplace(GetInstance().Damage_Arcane, GetInstance().Attributes_Resistance_Arcane);
	GetInstance().DamageTagToResistanceTag.Emplace(GetInstance().Damage_Physical, GetInstance().Attributes_Resistance_Physical);
}

void FAuraGmaeplayTags::InitEffectTags()
{
	GetInstance().EffectHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("伤害标签")
	);

}

void FAuraGmaeplayTags::InitSkillTags()
{
	GetInstance().Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("基础攻击")
	);
}

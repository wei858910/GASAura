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
	/*
 * Primary Attributes
 */
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

	/*
	 * Secondary Attributes
	 */

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

	/*输入*/
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


	GetInstance().Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("伤害标签")
	);

	GetInstance().EffectHitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("伤害标签")
	);

}

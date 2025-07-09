// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/*单例*/

struct FAuraGmaeplayTags
{

    static FAuraGmaeplayTags& GetInstance();
    static void InitNativeGameplayTags();

    FGameplayTag Attributes_Primary_Strength;
    FGameplayTag Attributes_Primary_Intelligence;
    FGameplayTag Attributes_Primary_Resilience;
    FGameplayTag Attributes_Primary_Vigor;

    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
    FGameplayTag Attributes_Secondary_MaxHealth;
    FGameplayTag Attributes_Secondary_MaxMana;

    FGameplayTag Attributes_Meta_IncomingXP; // 即将增加的XP

    /*输入*/
    FGameplayTag InputTag;
    FGameplayTag InputTag_LMB;
    FGameplayTag InputTag_RMB;
    FGameplayTag InputTag_1; // 数字键盘1
    FGameplayTag InputTag_2;
    FGameplayTag InputTag_3;
    FGameplayTag InputTag_4;
    FGameplayTag InputTag_Passive;
    FGameplayTag InputTag_Passive_1;
    FGameplayTag InputTag_Passive_2;

    /*玩家交互状态*/
    FGameplayTag Player_Block_InputPressed; // 按下
    FGameplayTag Player_Block_InputHeld;    // 按住
    FGameplayTag Player_Block_InputReleased;
    FGameplayTag Player_Block_CursorTrace; // 光标跟踪

    /* 技能 */
    FGameplayTag Abilities;
    FGameplayTag Abilities_None;
    FGameplayTag Abilities_Attack;
    FGameplayTag Abilities_Summon;

    FGameplayTag Abilities_HitReact;

    FGameplayTag Abilities_Status;
    FGameplayTag Abilities_Status_Locked;
    FGameplayTag Abilities_Status_Eligible;
    FGameplayTag Abilities_Status_Unlocked;
    FGameplayTag Abilities_Status_Equipped;

    FGameplayTag Abilities_Type_Offensive;
    FGameplayTag Abilities_Type_Passive;
    FGameplayTag Abilities_Type_None;

    FGameplayTag Abilities_Fire_FireBlot;
    FGameplayTag Cooldown_Fire_FireBlot;

    FGameplayTag Abilities_Fire_FireBlast;
    FGameplayTag Cooldown_Fire_FireBlast;

    FGameplayTag Abilities_Lightning_Electrocute;
    FGameplayTag Cooldown_Lightning_Electrocute;

    FGameplayTag Abilities_Arcane;
    FGameplayTag Abilities_Arcane_ArcaneShards;

    FGameplayTag Abilities_Passive;
    FGameplayTag Abilities_Passive_HaloOfProtection; // 守护
    FGameplayTag Abilities_Passive_LifeSiphon;       // 生命吸取
    FGameplayTag Abilities_Passive_ManaSiphon;       // 法术吸取

    FGameplayTag CombatSocket_Weapon;
    FGameplayTag CombatSocket_LeftHand;
    FGameplayTag CombatSocket_RightHand;
    FGameplayTag CombatSocket_Tail;

    FGameplayTag Montage_Attack_1;
    FGameplayTag Montage_Attack_2;
    FGameplayTag Montage_Attack_3;
    FGameplayTag Montage_Attack_4;

    /**效果*/
    FGameplayTag EffectHitReact;

    FGameplayTag GameplayCue_FireBlast;

    FGameplayTag Damage; // 用于设置GE的伤害元属性值

    /*伤害类型*/
    TMap<FGameplayTag, FGameplayTag> DamageTagToResistanceTag; // 伤害类型tag映射相应抗性
    TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;     // 伤害类型tag映射对应debuff

    FGameplayTag Damage_Fire;
    FGameplayTag Damage_Lightning; // 电伤
    FGameplayTag Damage_Arcane;    // 奥术伤害
    FGameplayTag Damage_Physical;  // 物理伤害

    // 攻击类型对应的抗性
    FGameplayTag Attributes_Resistance;
    FGameplayTag Attributes_Resistance_Fire;
    FGameplayTag Attributes_Resistance_Lightning;
    FGameplayTag Attributes_Resistance_Arcane;
    FGameplayTag Attributes_Resistance_Physical;

    // debuff
    FGameplayTag Debuff;
    FGameplayTag Debuff_Burn;     // 灼烧
    FGameplayTag Debuff_Stun;     // 眩晕 雷电造成
    FGameplayTag Debuff_Arcane;   // 奥术
    FGameplayTag Debuff_Physical; // 物理 流血？

    FGameplayTag Debuff_Chance; // 执行几率
    FGameplayTag Debuff_Damage;
    FGameplayTag Debuff_Duration;  // 持续周期
    FGameplayTag Debuff_Frequency; // 执行频率

private:
    void InitPrimaryAttributeTags();
    void InitSecondaryAttributeTags();
    void InitInputTags();
    void InitDamageTypesTags();
    void InitEffectTags();
    void InitSkillTags();
};
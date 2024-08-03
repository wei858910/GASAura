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
	FAuraGmaeplayTags::GetInstance().Attribute_Secondary_Armor=UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("减少所受伤害，提高格挡几率"));//CPP中像项目添加Tag的方式

}

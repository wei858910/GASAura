#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM()
enum ESaveSlotStatus:uint8 //存档插槽界面处于哪种界面模式
{
	Vacant=0, // 空缺插槽模式
	EnterName, //输入存档名
	Taken, //能直接加载
};

/*
 * 用以辅助存档技能
 */
USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults", DisplayName = "技能类")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,DisplayName="技能标签",meta=(Categories = "Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "技能状态", meta = (Categories = "Abilities.Status"))
	FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "技能装备槽", meta = (Categories = "InputTag"))
	FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "技能类型", meta = (Categories = "Abilities.Type"))
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "技能等级")
	int32 AbilityLevel;

};
inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}
/**
 * 存档，无多说
 */
UCLASS()
class AURA_LEARN_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName{};//对应界面中的插槽UI

	UPROPERTY()
	uint8 SlotIndex;

	UPROPERTY()
	FString PlayerName{"Default Name"};

	UPROPERTY()
	FString MapName{ "Default Name" };

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus{Vacant};

	UPROPERTY()
	bool bFirstTimeLoadIn{ true };//是否为第一次载入

	/* 玩家信息 */
	UPROPERTY()
	uint16 PlayerLevel{ 1 };

	UPROPERTY()
	uint32 PlayerXP{ 0 };

	UPROPERTY()
	uint16 SpellPoint{ 0 };//技能点

	UPROPERTY()
	uint16 AttributePoint{ 0 };

	UPROPERTY()
	float Strength{ 0 };

	UPROPERTY()
	float Vigor{ 0 };

	UPROPERTY()
	float Resilience{ 0 };

	UPROPERTY()
	float Intelligence{ 0 };
	/* 玩家信息 */

	/* 技能些 */
	UPROPERTY()
	TArray<FSavedAbility> SaveAbilities;
	/* 技能些 */
};

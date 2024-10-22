// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilitieDescriptions;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_LEARN_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category="Character Class Defaults",DisplayName="职业类型信息")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "技能信息")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly,Category="存档",DisplayName="存档类")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot,uint8 SlotIdx) const;
	static void DeleteSlotData(UMVVM_LoadSlot* LoadSlot, uint8 SlotIdx);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName,int32 SlotIdx) const;
};

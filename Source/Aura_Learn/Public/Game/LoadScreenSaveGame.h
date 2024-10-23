#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM()
enum ESaveSlotStatus:uint8 //存档插槽界面处于哪种界面模式
{
	Vacant=0, // 空缺插槽模式
	EnterName, //输入存档名
	Taken, //能直接加载
};

/**
 * 
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
	TEnumAsByte<ESaveSlotStatus> SlotStatus{Vacant};
};

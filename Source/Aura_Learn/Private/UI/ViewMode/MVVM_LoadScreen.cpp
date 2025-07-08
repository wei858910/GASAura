#include "UI/ViewMode/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitLoadSlot()
{
	InitLoadSlotItem(LoadSlot_0, 0);
	InitLoadSlotItem(LoadSlot_1, 1);
	InitLoadSlotItem(LoadSlot_2, 2);
}

void UMVVM_LoadScreen::InitLoadSlotItem(TObjectPtr<UMVVM_LoadSlot>& LoadSlotRef, const uint8 Index)
{
	const FString Slot{ "Load_Slot_" };

	LoadSlotRef = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlotRef->SetLoadSlotName(Slot + "0");
	LoadSlotRef->SlotIndex = 0;
	LoadSlots.Emplace(0, LoadSlotRef);
	
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetSlotViewModelByIndex(const uint8 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotBtnPressed(int32 idx, const FString& EnterName)
{
	if(auto AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[idx]->SetPlayerLevel(1);
		LoadSlots[idx]->SetPlayerName(EnterName);
		LoadSlots[idx]->SetMapName(AuraGameMode->DefaultMapName);
		LoadSlots[idx]->MapAssetName = AuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
		AuraGameMode->SaveSlotData(LoadSlots[idx], idx);
		LoadSlots[idx]->SlotStatus = ESaveSlotStatus::Taken;
		LoadSlots[idx]->InitSlostIndex();

		auto AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
		AuraGameInstance->LoadSlotName = LoadSlots[idx]->GetLoadSlotName();
		AuraGameInstance->LoadSlotIdx = LoadSlots[idx]->SlotIndex;
		AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	}
}

void UMVVM_LoadScreen::NewGameBtnPressed(int32 idx)
{
	LoadSlots[idx]->SetWgtSwitcherIndexDel.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotBtnPressed(int32 idx)
{
	HasSlotSelectedDel.Broadcast();
	ActiveSlot = LoadSlots[idx];
	for(auto& LoadSlot :LoadSlots)
	{
		//禁用按下的 加载按钮 开启另外两个槽的加载按钮
		if(LoadSlot.Key!=idx)
		{
			LoadSlot.Value->EnableSelectSlotBtnDel.Broadcast(true);
		}else
		{
			LoadSlot.Value->EnableSelectSlotBtnDel.Broadcast(false);
		}
	}
}

void UMVVM_LoadScreen::DeleteBtnPressed()
{
	if(IsValid(ActiveSlot))
	{
		AAuraGameModeBase::DeleteSlotData(ActiveSlot, ActiveSlot->SlotIndex);
		ActiveSlot->SlotStatus = ESaveSlotStatus::Vacant;
		ActiveSlot->InitSlostIndex();
		ActiveSlot->EnableSelectSlotBtnDel.Broadcast(true);
		if (auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			ActiveSlot->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
		}
		
		ActiveSlot = nullptr;
	}
	
}

void UMVVM_LoadScreen::OnPlayBtnPressed()
{
	if (auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		auto AuraGI=Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
		if(IsValid(ActiveSlot))
		{
			AuraGI->PlayerStartTag = ActiveSlot->PlayerStartTag;
			AuraGI->LoadSlotName = ActiveSlot->GetLoadSlotName();
			AuraGI->LoadSlotIdx = ActiveSlot->SlotIndex;

			AuraGameMode->TravelToMap(ActiveSlot);
		}
	}
}

void UMVVM_LoadScreen::LoadData()
{
	if(auto AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		for(const auto& LoadSlot:LoadSlots)
		{
			auto SaveObj = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

			LoadSlot.Value->SlotStatus = SaveObj->SlotStatus;
			LoadSlot.Value->SetPlayerName(SaveObj->PlayerName);
			LoadSlot.Value->SetPlayerLevel(SaveObj->PlayerLevel);
			LoadSlot.Value->SetMapName(SaveObj->MapName);
			LoadSlot.Value->PlayerStartTag = SaveObj->PlayerStartTag;
			LoadSlot.Value->InitSlostIndex();
		}
	}
}

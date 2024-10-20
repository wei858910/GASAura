#include "UI/ViewMode/MVVM_LoadScreen.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"

#define CREATE_LOAD_SLOT(INDEX,SlostStr)\
LoadSlot_##INDEX = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);\
LoadSlot_##INDEX->SetLoadSlotName( SlostStr+#INDEX);\
LoadSlots.Emplace(INDEX, LoadSlot_##INDEX)

void UMVVM_LoadScreen::InitLoadSlot()
{
	FString Slot{ "Load_Slot_" };

	CREATE_LOAD_SLOT(0, Slot);
	CREATE_LOAD_SLOT(1, Slot);
	CREATE_LOAD_SLOT(2, Slot);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetSlotViewModelByIndex(const uint8 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotBtnPressed(int32 idx, const FString& EnterName)
{
	if(auto AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[idx]->SetPlayerName(EnterName);
		AuraGameMode->SaveSlotData(LoadSlots[idx], idx);

		LoadSlots[idx]->InitSlostIndex();
	}
}

void UMVVM_LoadScreen::NewGameBtnPressed(int32 idx)
{
	LoadSlots[idx]->SetWgtSwitcherIndexDel.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotBtnPressed(int32 idx)
{

}

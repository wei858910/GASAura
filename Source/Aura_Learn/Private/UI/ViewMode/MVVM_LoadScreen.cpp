#include "UI/ViewMode/MVVM_LoadScreen.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"

#define CREATE_LOAD_SLOT(INDEX)\
LoadSlot_##INDEX = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);\
LoadSlots.Emplace(INDEX, LoadSlot_##INDEX)

void UMVVM_LoadScreen::InitLoadSlot()
{
	CREATE_LOAD_SLOT(0);
	CREATE_LOAD_SLOT(1);
	CREATE_LOAD_SLOT(2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetSlotViewModelByIndex(const uint8 Index) const
{
	return LoadSlots.FindChecked(Index);
}

 #include "UI/ViewMode/MVVM_LoadSlot.h"

 void UMVVM_LoadSlot::InitSlostIndex()
 {

	 SetWgtSwitcherIndexDel.Broadcast(2);
 }

 void UMVVM_LoadSlot::SetLoadSlotName(const FString& TargetName)
 {
	 UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName,TargetName);
 }

 void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
 {
	 UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
 }

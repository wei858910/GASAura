#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWgtSwitcherIndexDel,int32,WgtSwitcherIndex);

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void InitSlostIndex();

	void SetLoadSlotName(const FString& TargetName);
	void SetPlayerName(FString InPlayerName);

	FString GetPlayerName() const { return PlayerName; }
	const FString& GetLoadSlotName()const { return LoadSlotName; };

	UPROPERTY(BlueprintAssignable)
	FSetWgtSwitcherIndexDel SetWgtSwitcherIndexDel;//切换堆叠界面的显示成员


	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString PlayerName;//输入的存档名字

	UPROPERTY()
	uint8 SlotIndex;//

private:
	/*
	 * FieldNotify 启用对属性的变更通知。UE5引入，更轻松地跟踪和响应属性值的变化，尤其是在数据绑定和MVVM模式下使用时。
	 * FieldNotify的变量 被Setter、Getter修饰，必须存在Set和Get函数
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess="true"),DisplayName="插槽ID")
	FString LoadSlotName;//存档槽的名字
};

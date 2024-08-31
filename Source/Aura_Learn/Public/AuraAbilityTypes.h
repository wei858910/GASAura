#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameEffectContext :public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); };
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;//网络序列化

	virtual FAuraGameEffectContext* Duplicate() const override
	{
		FAuraGameEffectContext* NewContext = new FAuraGameEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	};

	bool IsCriticalHit() const { return bIsCriticalHit; };
	bool IsBlockedHit() const { return bIsBlockedHit; };
	void SetCriticalHit(const bool bValue) { bIsCriticalHit = bValue; };
	void SetBlockedHit(const bool bValue) { bIsBlockedHit = bValue; };

protected:
	UPROPERTY()
	bool bIsCriticalHit{ false };//是否暴击
	UPROPERTY()
	bool bIsBlockedHit{ false };//是否格挡
private:

};

template<>
struct  TStructOpsTypeTraits<FAuraGameEffectContext>:TStructOpsTypeTraitsBase2<FAuraGameEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// 以便将 TSharedPtr<FHitResult> 数据复制到各处
	};

};
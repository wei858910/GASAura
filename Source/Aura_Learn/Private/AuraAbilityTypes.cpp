#include "AuraAbilityTypes.h"

UScriptStruct* FAuraGameEffectContext::GetScriptStruct() const
{
	return FGameplayEffectContext::GetScriptStruct();
}

bool FAuraGameEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	/**
	 * 二进制 的 与& 或| 位<< >> 运算
	 * 用RepBit变量当作标识，该变量每个位标识是否有效  类似linux的selcet用fd的位标识是否有事件
	 */

	uint32 RepBits{ 0 };
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}

		//该类自己的数据序列化
		if(bIsBlockedHit)
		{
			RepBits |= 1 << 7;//从低位数的第七位标识 是否格挡
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}

	}
	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	//该类变量
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
		
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
		
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // 
	}

	bOutSuccess = true;
	return true;
}

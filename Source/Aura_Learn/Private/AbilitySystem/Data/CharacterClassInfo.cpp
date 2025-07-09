// 学习使用


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterType)
{
    return *CharacterClassInformation.Find(CharacterType);
}
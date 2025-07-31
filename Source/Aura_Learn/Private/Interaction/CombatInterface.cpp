// 学习使用


#include "Interaction/CombatInterface.h"

FHitResult* ICombatInterface::GetCursorHitRes()
{
    return nullptr;
}

bool ICombatInterface::IsHeroCharacter() const
{
    return false;
}

bool ICombatInterface::GetIsStunned()
{
    return false;
}
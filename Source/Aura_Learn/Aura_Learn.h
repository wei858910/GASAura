#pragma once

#include "CoreMinimal.h"
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1 //自定义的飞射物检测通道
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2 //对鼠标响应

namespace Aura
{
    extern const uint16 CUSTOM_DEPTH_RED;
    extern const uint16 CUSTOM_DEPTH_BLUE; //蓝色轮廓的值
    extern const uint16 CUSTOM_DEPTH_TAN;  //浅棕色轮廓的值
}
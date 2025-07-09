// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_LEARN_API UAuraAssetManager : public UAssetManager
{
    GENERATED_BODY()

public:
    static UAuraAssetManager& GetInstance();

protected:
    virtual void StartInitialLoading() override; //引擎初始化期间被调用 旨在处理游戏启动时的初始资源加载
};
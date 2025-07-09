#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class AURA_LEARN_API IPlayerInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /*防止playstate和Attribute互相嵌套引用，同时也能绕过GameMode 获取玩家状态的值*/
    UFUNCTION(BlueprintNativeEvent)
    int32 FindLevelFromXP(int32 XP) const;

    UFUNCTION(BlueprintNativeEvent)
    int32 GetXP() const;

    UFUNCTION(BlueprintNativeEvent)
    void AddToXP(int32 InXP);

    UFUNCTION(BlueprintNativeEvent)
    void LevelUp();

    UFUNCTION(BlueprintNativeEvent)
    int32 GetAttributePointReward(const int32 Level) const; //获取该级的升级奖励——属性点。

    UFUNCTION(BlueprintNativeEvent)
    int32 GetSpellPointReward(const int32 Level) const; //获取该级的升级奖励——法力点。

    UFUNCTION(BlueprintNativeEvent)
    void AddToPlayerLevel(int32 InPlayerLevel) const;

    UFUNCTION(BlueprintNativeEvent)
    void AddToAttributePoints(int32 InAttributePoint) const;

    UFUNCTION(BlueprintNativeEvent)
    int32 GetCurrentAttributePoint() const; //获取当前未用的属性点数

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void AddToSpellPoints(int32 InSpellPoint) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    int32 GetCurrentSpellPoint() const; //获取当前的咒法点数

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void ShowMagicCricle(UMaterialInterface* DecalMaterial = nullptr);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void HideMagicCricle();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SaveProgress(const FName& PlayerStartName); //储存进度

};
// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttributeTag{};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeName{};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeDescription{};

    UPROPERTY(BlueprintReadOnly)
    float AttributeValue{ 0 };

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayAttribute AttributeGetter; //标签对应的属性的获取 代替教程UAuraAttributeSet()中TagsToAttributes添加回调函数
};

UCLASS()
class AURA_LEARN_API UAttributeInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    /*
     *  @Tag 查找的目标Tag
     *  @bLogNotFound 是否用Log记录错误
     */
    FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAuraAttributeInfo> AttributeInformation;

};
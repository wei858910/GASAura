// 学习使用

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    const UInputAction* InputAction{ nullptr };

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag InputTag{};
};

UCLASS()
class AURA_LEARN_API UAuraInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAuraInputAction> AbilityInputActions;
};
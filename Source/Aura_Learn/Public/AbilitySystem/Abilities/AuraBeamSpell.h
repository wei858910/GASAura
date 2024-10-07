#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 类似光束类技能
 */
UCLASS()
class AURA_LEARN_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOnwerVaribles();

	/*检测电柱碰到的第一个人*/
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
protected:
	UPROPERTY(BlueprintReadWrite, Category="光束")
	FVector MouseHitLocation{FVector::Zero()};

	UPROPERTY(BlueprintReadWrite, Category = "光束")
	TObjectPtr<AActor> MouseHitActor{nullptr};

	UPROPERTY(BlueprintReadWrite, Category = "光束")
	TObjectPtr<APlayerController> OnwerPlayerController{nullptr};

	UPROPERTY(BlueprintReadWrite, Category = "光束")
	TObjectPtr<ACharacter> OnwerCharacter{ nullptr };
};

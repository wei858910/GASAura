#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

const auto DebuffClamp = FAuraGmaeplayTags::GetInstance().Debuff.ToString();

/* 在组件基础上添加 tag 以方便区分触发和禁用*/
UCLASS()
class AURA_LEARN_API UDebuffNiagaraComponent : public UNiagaraComponent
{
    GENERATED_BODY()

public:
    UDebuffNiagaraComponent();

    UPROPERTY(VisibleAnywhere) /*, meta = (Categories = DebuffClamp)*/
    FGameplayTag DebuffTag{};

protected:
    virtual void BeginPlay() override;

    void DebuffTagCahnged(const FGameplayTag CallackTag, const int32 NewCount);

    UFUNCTION()
    void OnOnwerDead(AActor* DeadActor);
};
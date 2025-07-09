// 学习使用


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::GetInstance()
{
	check(GEngine)
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGmaeplayTags::InitNativeGameplayTags();

}

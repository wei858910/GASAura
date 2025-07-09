#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura_Learn/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewMode/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, uint8 SlotIdx) const
{
    if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIdx))
    {
        //已经存在的存档 删除之后再保存一个新的
        UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIdx);
    }

    auto SaveGameObj = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    auto LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObj);

    LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
    LoadScreenSaveGame->SlotStatus = Taken;
    LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
    LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
    LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;

    UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIdx);
}

void AAuraGameModeBase::DeleteSlotData(UMVVM_LoadSlot* LoadSlot, uint8 SlotIdx)
{
    if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIdx))
    {
        UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIdx);
    }
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIdx) const
{
    USaveGame* SaveGameObj{ nullptr };
    if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIdx))
    {
        SaveGameObj = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIdx);

    }
    else
    {
        SaveGameObj = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    }

    return Cast<ULoadScreenSaveGame>(SaveGameObj);

}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
    auto AuraGI = Cast<UAuraGameInstance>(GetGameInstance());

    return GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIdx);

}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObj)
{
    auto AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
    AuraGI->PlayerStartTag = SaveObj->PlayerStartTag;
    UGameplayStatics::SaveGameToSlot(SaveObj, AuraGI->LoadSlotName, AuraGI->LoadSlotIdx);
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& AssetName)
{
    for (const auto& It : NameToMap)
    {
        if (It.Value.ToSoftObjectPath().GetAssetName() == AssetName)
        {
            return It.Key;
        }
    }
    return FString();
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& AssetNameOfTargetMap, const FName& StartTagOfTargetMap)
{
    FString WorldName = World->GetMapName();                 //该方法获得的lName会带有前缀
    WorldName.RemoveFromStart(World->StreamingLevelsPrefix); //移除地图的前缀名

    auto AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
    check(AuraGI);

    //为当前存档添加地图信息
    if (auto SaveSlot = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIdx))
    {
        //AssetNameOfTargetMap有值，那说明是从加载地图处调用，则保存下个地图的信息
        if (!AssetNameOfTargetMap.IsEmpty())
        {
            SaveSlot->MapAssetName = AssetNameOfTargetMap;
            SaveSlot->MapName = GetMapNameFromMapAssetName(AssetNameOfTargetMap);
            AuraGI->PlayerStartTag = StartTagOfTargetMap;
        }

        //不存在就进行添加
        if (!SaveSlot->HasMap(WorldName))
        {
            FSavedMap NewSaveMap;
            NewSaveMap.MapAssetName = WorldName;
            SaveSlot->SavedMaps.AddUnique(NewSaveMap);
        }

        auto SaveMap = SaveSlot->GetSavedMapWithMapName(WorldName);
        SaveMap.SavedActors.Empty();

        /*世界中的角色状态*/
        for (FActorIterator It(World); It; ++It)
        {
            AActor* Actor = *It;

            if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())
            {
                continue;
            }

            FSavedActor SavedActor;
            SavedActor.ActorName = Actor->GetFName();
            SavedActor.Transform = Actor->GetTransform();

            /* 对世界中的Actor们进行保存 */
            FMemoryWriter MemoryWriter(SavedActor.Bytes);                   //用以内存读写，方便序列化
            FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true); //序列化代理，用于Actor的所有属性和对象数据转成可保存的格式
            Archive.ArIsSaveGame = true;                                    //该存档处于保存或者加载状态，只有被标记为 SaveGame 的属性才会被序列化
            Actor->Serialize(Archive);

            SaveMap.SavedActors.AddUnique(SavedActor);
        }

        //找到对应的MAP ，将新的Map数据赋值
        for (FSavedMap& MapToReplace : SaveSlot->SavedMaps)
        {
            if (MapToReplace.MapAssetName == WorldName)
            {
                MapToReplace = SaveMap;
            }
        }
        UGameplayStatics::SaveGameToSlot(SaveSlot, AuraGI->LoadSlotName, AuraGI->LoadSlotIdx);
    }
}

void AAuraGameModeBase::LoadWorldState(UWorld* World)
{
    FString WorldName = World->GetMapName();                 //该方法获得的lName会带有前缀
    WorldName.RemoveFromStart(World->StreamingLevelsPrefix); //移除地图的前缀名

    auto AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
    check(AuraGI);

    if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIdx))
    {
        ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIdx));
        if (SaveGame == nullptr)
        {
            UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
            return;
        }

        for (FActorIterator It(World); It; ++It)
        {
            AActor* ItActor = *It;
            if (!IsValid(ItActor) || !ItActor->Implements<USaveInterface>())
            {
                continue;
            }

            for (const auto& LoadedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
            {
                if (LoadedActor.ActorName == ItActor->GetName())
                {
                    if (ISaveInterface::Execute_ShouldLoadTransform(ItActor))
                    {
                        ItActor->SetActorTransform(LoadedActor.Transform);
                    }

                    FMemoryReader MemoryReader(LoadedActor.Bytes);

                    FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
                    Archive.ArIsSaveGame = true;
                    ItActor->Serialize(Archive); // 将序列化数据设置为值

                    ISaveInterface::Execute_LoadActor(ItActor);
                }

            }
        }
    }

}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, NameToMap.FindChecked(Slot->GetMapName()));
}

void AAuraGameModeBase::PlayerDied(ACharacter* Deader)
{
    auto SaveGame = RetrieveInGameSaveData();
    if (!IsValid(SaveGame))
    {
        return;
    }

    UGameplayStatics::OpenLevel(Deader, FName(SaveGame->MapAssetName));
}

void AAuraGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    NameToMap.Emplace(DefaultMapName, DefaultMap);
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    auto AuraGI = Cast<UAuraGameInstance>(GetGameInstance());

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
    if (Actors.Num() > 0)
    {
        AActor* SelectedActor = Actors[0];
        for (AActor* Actor : Actors)
        {
            if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
            {
                //根据Tag找到所需的起始点
                if (PlayerStart->PlayerStartTag == AuraGI->PlayerStartTag)
                {
                    SelectedActor = PlayerStart;
                    break;
                }
            }
        }
        return SelectedActor;
    }
    return nullptr;
}
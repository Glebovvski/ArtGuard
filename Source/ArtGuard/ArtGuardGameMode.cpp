// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGuardGameMode.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Engine/ObjectLibrary.h"
#include "Room.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Area.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Robber.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

void AArtGuardGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArtGuardGameMode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	IsRightExitSet = false;
	IsUpExitSet = false;
	GetAllMaterials();
}

TArray<UMaterialInstance*> AArtGuardGameMode::GetVerticalMaterials() const
{
	return VerticalMaterials;
}

TArray<UMaterialInstance*> AArtGuardGameMode::GetHorizontalMaterials() const
{
	return HorizontalMaterials;
}

void AArtGuardGameMode::RemoveUsedPictureFromArray(bool IsHorizontalArray, int index)
{
	if (IsHorizontalArray)
		HorizontalMaterials.RemoveAt(index);
	else VerticalMaterials.RemoveAt(index);
}

void AArtGuardGameMode::SpawnArea()
{
	FVector Scale = FVector(150, 150, 1);
	FVector Location = FVector(Scale.X / 2 * 100, Scale.Y / 2 * 100, 0);
	FTransform RootTransform = FTransform(FRotator::ZeroRotator, Location, Scale);
	AArea* Root = Cast<AArea>(UGameplayStatics::BeginSpawningActorFromClass(this, BP_Area, RootTransform));
	if (Root)
	{
		Root->Width = RootTransform.GetScale3D().X;
		Root->Height = RootTransform.GetScale3D().Y;
		UGameplayStatics::FinishSpawningActor(Root, RootTransform);
	}
	Areas.Add(Root);
	int MinSize = Root->MIN_AREA_SIZE;

	while (Areas.Num() != 0)
	{
		auto Area = Areas[0];
		Areas.RemoveAt(0);

		if (Area->LeftAreaChild == nullptr && Area->RightAreaChild == nullptr)
		{
			if (Area->Split())
			{
				Area->SpawnChild();
				Areas.Add(Area->LeftAreaChild);
				Areas.Add(Area->RightAreaChild);
			}
		}
	}

	Root->CreateRooms();

	for (TActorIterator<AArea> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AArea* Area = *ActorItr;
		FoundAreas.Add(Area);
		Area->CreateHall();
	}

	for (AArea* Area : FoundAreas)
	{
		if (Area->Room)
			Area->Room->CreateWalls();
	}
}

AArea* AArtGuardGameMode::GetMainRightExit()
{
	return MainRightExit;
}

AArea* AArtGuardGameMode::GetMainUpExit()
{
	return MainUpExit;
}

void AArtGuardGameMode::SpawnRobber()
{
	float X = 0, Y = 0;
	bool check = false;
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	do
	{
		auto RandomRoomIndex = FMath::RandRange(0, FoundAreas.Num()-1);
		UE_LOG(LogTemp, Warning, TEXT("%d"), FoundAreas.Num());
		if (FoundAreas[RandomRoomIndex]->Room)
		{
			if (FVector::Distance(FoundAreas[RandomRoomIndex]->Room->Location, Player->GetActorLocation()) > 5000)
			{
				auto Room = FoundAreas[RandomRoomIndex]->Room;
				X = FMath::RandRange(Room->Location.X - Room->Width * 100 + 2000, Room->Location.X + Room->Width * 100 - 2000);
				Y = FMath::RandRange(Room->Location.Y - Room->Height * 100 + 2000, Room->Location.Y + Room->Height * 100 - 2000);
				check = true;
			}
		}
	} while (!check);

	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector(X, Y, 150));
	ARobber* Robber = Cast<ARobber>(UGameplayStatics::BeginSpawningActorFromClass(GetWorld(), BP_Robber, SpawnTransform, false, this));
	if (Robber)
	{
		UGameplayStatics::FinishSpawningActor(Robber, SpawnTransform);
	}
	//RobberAIController=Robber->GetController()->GetPawn()->AIControllerClass;
	Perception=Robber->GetPerception();
}

/*
void AArtGuardGameMode::SetSight(float SightRadius)
{
	 FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
 
     if (!Id.IsValid())
     {
         UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
         return;
     }
 
     // GetPerception() = AIController->GetPerceptionComponent()
    
	auto Config = Perception->GetSenseConfig(Id); //GetPerception()->GetSenseConfig(Id);
 
     if (Config == nullptr)
         return;
 
     auto ConfigSight = Cast<UAISenseConfig_Sight>(Config);
 
     // Save original lose range
     //float LoseRange = ConfigSight->LoseSightRadius - ConfigSight->SightRadius;
     
     ConfigSight->SightRadius = SightRadius;
 
     //RobberAIController->GetAIPerceptionComponent()->RequestStimuliListenerUpdate(); //->RequestStimuliListenerUpdate();
	 
}
*/

void AArtGuardGameMode::GetAllMaterials()
{
	VerticalMaterials = GetMaterialInstancesFromPath("/Game/Picture/Pictures/Materials/Vertical");
	HorizontalMaterials = GetMaterialInstancesFromPath("/Game/Picture/Pictures/Materials/Horizontal");
}

TArray<UMaterialInstance*> AArtGuardGameMode::GetMaterialInstancesFromPath(FString path)
{
	TArray<UMaterialInstance*> result = TArray<UMaterialInstance*>();

	UObjectLibrary* lib = UObjectLibrary::CreateLibrary(UMaterialInstance::StaticClass(), false, true);
	lib->LoadAssetDataFromPath(path);
	TArray<FAssetData> assetData;
	lib->GetAssetDataList(assetData);

	for (FAssetData asset : assetData)
	{
		UMaterialInstance* mi = Cast<UMaterialInstance>(asset.GetAsset());
		if (mi)
		{
			result.Add(mi);
		}
	}
	return result;
}

void AArtGuardGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	VerticalMaterials.Empty();
	HorizontalMaterials.Empty();
}
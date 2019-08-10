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

void AArtGuardGameMode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	IsRightExitSet=false;
	IsUpExitSet=false;
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
	FVector Scale = FVector(150,150,1);
	FVector Location = FVector(Scale.X/2*100, Scale.Y/2*100, 0);
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

	TArray<AArea*> FoundAreas;
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
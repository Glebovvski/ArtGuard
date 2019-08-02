// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtGuardGameMode.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Engine/ObjectLibrary.h"
#include "Room.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Cell.h"
#include "Grid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GridCell.h"

void AArtGuardGameMode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetAllMaterials();

	MaxCircleRooms = 1;
	CurrentCircleRooms = 0;
	MaxSmallRooms = 9;
	CurrentSmallRooms = 0;
	MaxRectRooms = 9;
	CurrentRectRooms = 0;
	MaxBigRooms = 5;
	CurrentBigRooms = 0;

	RoomTypes.Add(SmallSquareRoom_BP);
	RoomTypes.Add(BigSquareRoom_BP);
	RoomTypes.Add(RectangleRoom_BP);
	RoomTypes.Add(RectangleVerticalRoom_BP);
	RoomTypes.Add(CircleRoom_BP);

	RoomsCount.Add(CircleRoom_BP, CurrentCircleRooms);
	RoomsCount.Add(RectangleRoom_BP, CurrentRectRooms);
	RoomsCount.Add(SmallSquareRoom_BP, CurrentSmallRooms);
	RoomsCount.Add(BigSquareRoom_BP, CurrentBigRooms);
	RoomsCount.Add(RectangleVerticalRoom_BP, CurrentRectRooms);
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

void AArtGuardGameMode::SetGrid(AGrid* GridToSet)
{
	Grid = GridToSet;
}

int AArtGuardGameMode::ConfigureCellToSpawnOn(ERoomPosition Position)
{
	int IndexCell = 0;
	switch (Position)
	{
	case ERoomPosition::CENTRE_RIGHT:
	case ERoomPosition::CENTRE_LEFT:
	case ERoomPosition::CENTRE:
	case ERoomPosition::BOTTOM_CENTRE:
	case ERoomPosition::BOTTOM_LEFT:
	case ERoomPosition::BOTTOM_RIGHT:
		IndexCell = 0;
		break;
	case ERoomPosition::UPPER_CENTRE:
	case ERoomPosition::UPPER_LEFT:
	case ERoomPosition::UPPER_RIGHT:
		IndexCell = 2;
		break;
	}
	return IndexCell;
}


void AArtGuardGameMode::SpawnExtraRooms()
{
	for (int i = 0; i < Grid->Grid.Num(); i++)
	{
		if (!Grid->Grid[i]->IsUsed)
		{
			SpawnToTheEmptyCellsLeft(Grid->Grid[i]);
		}
	}

	for (ARoom* Room : SpawnedRooms)
	{
		Room->GetExits(EOtherRoomPosition::UP);
		Room->GetExits(EOtherRoomPosition::BOTTOM);
		Room->GetExits(EOtherRoomPosition::RIGHT);
		Room->GetExits(EOtherRoomPosition::LEFT);
		/*
		if(Room->LeftExitArrows.Num()==3)
		{
			Room->LeftExitArrows.RemoveAt(0);
			Room->LeftExitArrows.RemoveAt(0);
		}
		if(Room->RightExitArrows.Num()==3)
		{
			Room->RightExitArrows.RemoveAt(0);
			Room->RightExitArrows.RemoveAt(0);
		}
		if(Room->UpMainExitArrows.Num()==3)
		{
			Room->UpMainExitArrows.RemoveAt(0);
			Room->UpMainExitArrows.RemoveAt(0);
		}
		if(Room->BottomMainExitArrows.Num()==3)
		{
			Room->BottomMainExitArrows.RemoveAt(0);
			Room->BottomMainExitArrows.RemoveAt(0);
		}
		*/
		Room->SpawnExits();
	}
}


void AArtGuardGameMode::SpawnRooms()
{
	for (ERoomPosition Position : Positions)
	{
		GetFreeCellsOnPosition(Position);
		if (RemainedCells.Num() > 0)
		{
			TSubclassOf<ARoom> RoomToSpawn;
			do
			{
				RoomToSpawn = RoomTypes[FMath::RandRange(0, RoomTypes.Num() - 1)];
				RoomsCount[RoomToSpawn]++;
				if (RoomsCount[RoomToSpawn] > GetMaxAmountOfRoomsForType(RoomToSpawn))
				{
					RoomTypes.Remove(RoomToSpawn);
					RoomToSpawn = nullptr;
				}
			} while (!RoomToSpawn);


			SpawnRoom(RemainedCells[ConfigureCellToSpawnOn(Position)], RoomToSpawn);
		}
	}

	SpawnExtraRooms();


	//auto Player = GetWorld()->GetFirstPlayerController();
	//Player->GetPawn()->SetActorLocation(RemainedCells[0]->GetActorLocation() + FVector(50, 50, 110));

	//SpawnRoom(Grid->Grid[0], CircleRoom_BP);
}

ARoom* AArtGuardGameMode::SpawnRoom(ACell* Cell, TSubclassOf<ARoom> RoomToSpawn)
{
	int index = Grid->Grid.Find(Cell);
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	ARoom* Room = GetWorld()->SpawnActor<ARoom>(RoomToSpawn, FVector(0, 0, 1000), FRotator(0));
	Room->RoomPosition = Cell->RoomPosition;

	auto Location = Cell->GetActorLocation();
	Room->AttachToActor(Cell, FAttachmentTransformRules::KeepRelativeTransform, "");
	FVector RoomLocation;

	if (RoomToSpawn == CircleRoom_BP || RoomToSpawn == BigSquareRoom_BP)
	{
		RoomLocation = FVector(-Cell->Plane->GetComponentScale().X / 2 * 100 /*+ Grid->offset / 2*/, Cell->Plane->GetComponentScale().Y / 2 * 100 + Grid->offset / 2, 100);
		Grid->Grid[index + Grid->size]->AttachToActor(Cell, AttachmentRules, "");
		Grid->Grid[index + 1]->AttachToActor(Cell, AttachmentRules, "");
		Grid->Grid[index + Grid->size + 1]->AttachToActor(Cell, AttachmentRules, "");
	}
	else if (RoomToSpawn == RectangleRoom_BP)
	{
		RoomLocation = FVector(-Cell->Plane->GetComponentScale().X / 2 * 100 /*+ Grid->offset / 2*/, 0, 100);
		Grid->Grid[index + 1]->AttachToActor(Cell, AttachmentRules, "");
	}
	else if (RoomToSpawn == RectangleVerticalRoom_BP)
	{
		RoomLocation = FVector(-Cell->Plane->GetComponentScale().X / 2 * 100 , Cell->Plane->GetComponentScale().Y / 2 * 100 + Grid->offset / 2, 100);
		Grid->Grid[index + Grid->size]->AttachToActor(Cell, AttachmentRules, "");
	}
	else if (RoomToSpawn == SmallSquareRoom_BP)
		RoomLocation = FVector(-Cell->Plane->GetComponentScale().X / 2 * 100, 0, 100);

	Room->SetActorRelativeLocation(RoomLocation);
	SpawnedRooms.Add(Room);
	return Room;
	//else UE_LOG(LogTemp, Warning, TEXT("ROOM IS NULL: %s"));
}

void AArtGuardGameMode::SpawnToTheEmptyCellsLeft(ACell* Cell)
{
	int Index = Grid->Grid.Find(Cell);
	bool bIsLast = Index >= Grid->Grid.Num() - Grid->size ? true : false;
	int BelowCurrent = Index + Grid->size;
	int NextIndex = Index + 1;

	if (!bIsLast && !Grid->Grid[Index + Grid->size]->IsUsed && Index < Grid->Grid.Num() - Grid->size)
	{
		auto Room = SpawnRoom(Cell, RectangleVerticalRoom_BP);
	}
	else if (NextIndex < Grid->Grid.Num() && !Grid->Grid[Index + 1]->IsUsed && (Index + 1) % Grid->size != 0)// ||  !Grid->Grid[Index+1]->IsUsed && (Index + 1) % Grid->size != 0)
	{
		//spawn rectangle without rotation
		auto Room = SpawnRoom(Cell, RectangleRoom_BP);
		//UE_LOG(LogTemp, Warning, TEXT("ROOM: %s"), *Room->GetName());
	}
	else
	{
		auto Room = SpawnRoom(Cell, SmallSquareRoom_BP);
		//UE_LOG(LogTemp, Warning, TEXT("ROOM: %s"), *Room->GetName());
	}

}

void AArtGuardGameMode::GetFreeCellsOnPosition(ERoomPosition RoomPosition)
{
	RemainedCells.Empty();
	for (ACell* Cell : Grid->Grid)
	{
		if (!Cell->IsUsed && Cell->RoomPosition == RoomPosition)
		{
			RemainedCells.Add(Cell);
		}
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

int AArtGuardGameMode::GetMaxAmountOfRoomsForType(TSubclassOf<ARoom> RoomType)
{
	if (RoomType == BigSquareRoom_BP) return MaxBigRooms;
	else if (RoomType == SmallSquareRoom_BP) return MaxSmallRooms;
	else if (RoomType == RectangleRoom_BP || RoomType == RectangleVerticalRoom_BP) return MaxRectRooms;
	else return MaxCircleRooms;
}

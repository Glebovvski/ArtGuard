// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GameFramework/Actor.h"
#include "Cell.h"
#include "Components/StaticMeshComponent.h"
#include "ArtGuardGameMode.h"
#include "Components/BoxComponent.h"
// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGrid::SpawnGrid()
{
	AArtGuardGameMode* GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			FVector Scale = FVector(20,20,0);
			ACell* Cell = GetWorld()->SpawnActor<ACell>(Cell_BP, FVector(j * Scale.X * 100 + j * offset, i * Scale.Y * 100 + i * offset, -100), FRotator(0));
			//FVector Scale = Cell->Plane->GetComponentScale();
			//Cell->SetActorLocation(FVector(j * Scale.X * 100 + j * offset, i * Scale.Y * 100 + i * offset, -100));

			//Cell->CollisionCheck->OnComponentBeginOverlap.AddDynamic(Cell, &ACell::OnOverlapBegin);
			//Cell->CollisionCheck->OnComponentEndOverlap.AddDynamic(Cell, &ACell::OnOverlapEnd);

			if (i >= 0 && i <= size / 3 - 1)
			{
				if (j >= 0 && j <= size / 3 - 1)
					Cell->RoomPosition = ERoomPosition::BOTTOM_LEFT;
				else if (j >= size / 3 && j <= size / 3 + 1)
					Cell->RoomPosition = ERoomPosition::BOTTOM_CENTRE;
				else if (j >= size / 3 + 1 && j <= size)
					Cell->RoomPosition = ERoomPosition::BOTTOM_RIGHT;
			}

			if (i >= size / 3 && i <= size / 3 + 1)
			{
				if (j >= 0 && j <= size / 3 - 1)
					Cell->RoomPosition = ERoomPosition::CENTRE_LEFT;
				else if (j >= size / 3 && j <= size / 3 + 1)
					Cell->RoomPosition = ERoomPosition::CENTRE;
				else if (j >= size / 3 + 1 && j <= size)
					Cell->RoomPosition = ERoomPosition::CENTRE_RIGHT;
			}

			if (i >= size / 3 + 1 && i <= size)
			{
				if (j >= 0 && j <= size / 3 - 1)
					Cell->RoomPosition = ERoomPosition::UPPER_LEFT;
				else if (j >= size / 3 && j <= size / 3 + 1)
					Cell->RoomPosition = ERoomPosition::UPPER_CENTRE;
				else if (j >= size / 3 + 1 && j <= size)
					Cell->RoomPosition = ERoomPosition::UPPER_RIGHT;
			}

			Grid.Add(Cell);
		}
	}
	//SetOverlappingEventsOnCells();
}

void AGrid::SetOverlappingEventsOnCells()
{
	for (ACell* Cell : Grid)
	{
		Cell->CollisionCheck->OnComponentBeginOverlap.AddDynamic(Cell, &ACell::OnOverlapBegin);
		Cell->CollisionCheck->OnComponentEndOverlap.AddDynamic(Cell, &ACell::OnOverlapEnd);
	}
}

bool AGrid::IsInRange(int Number, int Min, int Max)
{
	if (Number >= Min && Number <= Max)
		return true;
	else return false;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


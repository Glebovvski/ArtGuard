// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Wall.h"
#include "Exit.h"
#include "Frame.h"
#include "Components/ArrowComponent.h"
#include "ArtGuardGameMode.h"
#include "Components/BoxComponent.h"
#include "Cell.h"

// Sets default values
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();

	UpCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnOverlapBeginUp);
	BottomCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnOverlapBeginBottom);
	RightCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnOverlapBeginRight);
	LeftCollision->OnComponentBeginOverlap.AddDynamic(this, &ARoom::OnOverlapBeginLeft);
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoom::SetFloor(UStaticMeshComponent* FloorToSet)
{
	Floor = FloorToSet;
}


void ARoom::SetGameMode()
{
	GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
}

void ARoom::CastToArrowArray(TArray<USceneComponent*> Array, TArray<UArrowComponent*>& OutArray)
{
	for (USceneComponent* Arrow : Array)
	{
		OutArray.Add(Cast<UArrowComponent>(Arrow));
	}
}

void ARoom::SetArrows(TArray<USceneComponent*> UpMainArrowsToSet, TArray<USceneComponent*> BottomMainArrowsToSet, TArray<USceneComponent*> LeftArrowsToSet, TArray<USceneComponent*> RightArrowsToSet)
{
	CastToArrowArray(UpMainArrowsToSet, UpMainExitArrows);
	CastToArrowArray(BottomMainArrowsToSet, BottomMainExitArrows);
	CastToArrowArray(LeftArrowsToSet, LeftExitArrows);
	CastToArrowArray(RightArrowsToSet, RightExitArrows);

}

void ARoom::SpawnWalls()
{
	FVector RoomScale = Floor->GetComponentScale();
	NumberOfWallsByX = (RoomScale.X / 5) * 2;
	NumberOfWallsByY = (RoomScale.Y / 5) * 2;
	int AdditionalOffsetX = RoomScale.X > 20 ? 100 : 0;
	int AdditionalOffsetY = RoomScale.Y > 20 ? 100 : 0;
	if (Floor)
	{
		FRotator FloorRotation = GetActorRotation();
		for (int i = 0; i < NumberOfWallsByX; i++)
		{
			AWall* Wall = GetWorld()->SpawnActor<AWall>(Wall_BP);
			FVector WallScale = Wall->Wall->GetComponentScale();

			if (i < NumberOfWallsByX / 2)
			{
				int offset = i * WallScale.X * 100 + AdditionalOffsetX;
				Wall->SetActorLocation(FVector((WallScale.X / 2) * 100 + offset, (RoomScale.Y / 2) * 100, 735));
				Wall->AddActorLocalRotation(FRotator(0, FloorRotation.Yaw + 180, 0));
				auto Frame = Wall->SpawnFrame();
				Frame->SpawnPicture();
			}
			else
			{
				int offset = (i - NumberOfWallsByX / 2) * WallScale.X * 100 + AdditionalOffsetX;
				Wall->SetActorLocation(FVector((WallScale.X / 2) * 100 + offset, -(RoomScale.Y / 2) * 100, 735));
				Wall->AddActorLocalRotation(FRotator(0, FloorRotation.Yaw, 0));
				auto Frame = Wall->SpawnFrame();
				Frame->SpawnPicture();
			}

			Wall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, "");
		}

		for (int i = 0; i < NumberOfWallsByY; i++)
		{
			AWall* Wall = GetWorld()->SpawnActor<AWall>(Wall_BP);
			FVector WallScale = Wall->Wall->GetComponentScale();

			if (i < NumberOfWallsByY / 2)
			{
				int offset = i * WallScale.X * 100 + AdditionalOffsetY;
				Wall->SetActorLocation(FVector(0, -RoomScale.Y / 2 * 100 + (WallScale.X / 2) * 100 + offset, 750));
				Wall->AddActorLocalRotation(FRotator(0, FloorRotation.Yaw + 90 + 180, 0));
				auto Frame = Wall->SpawnFrame();
				Frame->SpawnPicture();
			}
			else
			{
				int offset = (i - NumberOfWallsByY / 2) * WallScale.X * 100 + AdditionalOffsetY;
				Wall->SetActorLocation(FVector(RoomScale.X * 100, -RoomScale.Y / 2 * 100 + (WallScale.X / 2) * 100 + offset, 750));
				Wall->AddActorLocalRotation(FRotator(0, FloorRotation.Yaw + 90, 0));
				auto Frame = Wall->SpawnFrame();
				Frame->SpawnPicture();
			}

			Wall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, "");
		}
	}

	//AddActorWorldRotation(FRotator(0, -90, 0));
}


void ARoom::SpawnExits()
{
	//ConfigureRoomExits();
	//FVector RoomScale = Floor->GetComponentScale();

	AllExits.Add(UpMainExitArrows);
	AllExits.Add(BottomMainExitArrows);
	AllExits.Add(LeftExitArrows);
	AllExits.Add(RightExitArrows);

	for (auto Array : AllExits)
	{
		SpawnExit(Array);
	}
}


void ARoom::SpawnExit(TArray<UArrowComponent*> Array)
{
	for (int i = 0; i < Array.Num(); i++)
	{
		auto Exit = GetWorld()->SpawnActor<AExit>(Exit_BP);
		Exit->SetActorTransform(Array[i]->GetComponentTransform());
		Exit->AttachToComponent(Array[i], FAttachmentTransformRules::KeepWorldTransform, "");
		Exit->SetActorRelativeLocation(FVector(0));
		auto Aisle = GetWorld()->SpawnActor<AActor>(Aisle_BP);
		Aisle->SetActorTransform(Array[i]->GetComponentTransform());
		Aisle->AttachToComponent(Array[i], FAttachmentTransformRules::KeepRelativeTransform, "");
		Aisle->SetActorRotation(Array[i]->GetComponentRotation());
		Aisle->SetActorRelativeLocation(FVector(0));
	}
}

void ARoom::SpawnSideExits(TArray<UArrowComponent*> Array)
{

}

void ARoom::SetupCollisionBoxes(UBoxComponent* Up, UBoxComponent* Bottom, UBoxComponent* Right, UBoxComponent* Left)
{
	UpCollision = Up;
	BottomCollision = Bottom;
	RightCollision = Right;
	LeftCollision = Left;
}

ARoom* ARoom::GetRoom(EOtherRoomPosition RoomPosition) const
{
	switch (RoomPosition)
	{
	case EOtherRoomPosition::BOTTOM:
		return BottomRoom;
		break;
	case EOtherRoomPosition::LEFT:
		return LeftRoom;
		break;
	case EOtherRoomPosition::RIGHT:
		return RightRoom;
		break;
	case EOtherRoomPosition::UP:
		return UpRoom;
		break;
	default:
		return UpRoom;
	}
}

void ARoom::GetExits(EOtherRoomPosition RoomPosition)
{
	TArray<UArrowComponent*> Exits;
	TArray<UArrowComponent*> ExitsToRemove;
	ARoom* OtherRoom = GetRoom(RoomPosition);
	if (OtherRoom)
		UE_LOG(LogTemp, Warning, TEXT("%s CHECKS %s"), *this->GetName(), *OtherRoom->GetName());
	switch (RoomPosition)
	{
		case EOtherRoomPosition::UP:
		if (UpCount > 0)
		{
			Exits = OtherRoom->BottomMainExitArrows;
			for (int i = 0; i < UpMainExitArrows.Num(); i++)
			{
				if (Exits.Num() > i)
				{
					if (UpMainExitArrows[i]->GetComponentLocation().X != Exits[i]->GetComponentLocation().X)
					{
						ExitsToRemove.Add(UpMainExitArrows[i]);
					}
				}
				else ExitsToRemove.Add(UpMainExitArrows[i]);
			}
			RemoveRange(UpMainExitArrows, ExitsToRemove);
			RemoveRange(OtherRoom->BottomMainExitArrows, ExitsToRemove);
		}
		else
		{
			UpMainExitArrows.Empty();
		}
		break;

	case EOtherRoomPosition::BOTTOM:
		if (BottomCount > 0)
		{
			Exits = OtherRoom->UpMainExitArrows;
			for (int i = 0; i < BottomMainExitArrows.Num(); i++)
			{
				if (Exits.Num() > i)
				{
					if (BottomMainExitArrows[i]->GetComponentLocation().X != Exits[i]->GetComponentLocation().X)
					{
						ExitsToRemove.Add(BottomMainExitArrows[i]);
					}
				}
				else ExitsToRemove.Add(BottomMainExitArrows[i]);
			}
			RemoveRange(BottomMainExitArrows, ExitsToRemove);
			RemoveRange(OtherRoom->UpMainExitArrows, ExitsToRemove);
		}
		else
		{
			BottomMainExitArrows.Empty();
		}
		break;

		case EOtherRoomPosition::RIGHT:
		if (RightCount > 0)
		{
			Exits = OtherRoom->LeftExitArrows;
			for (int i = 0; i < RightExitArrows.Num(); i++)
			{
				if (Exits.Num() > i)
				{
					if (RightExitArrows[i]->GetComponentLocation().Y != Exits[i]->GetComponentLocation().Y)
					{
						ExitsToRemove.Add(RightExitArrows[i]);
					}
				}
				else ExitsToRemove.Add(RightExitArrows[i]);
			}
			RemoveRange(RightExitArrows, ExitsToRemove);
			RemoveRange(OtherRoom->LeftExitArrows, ExitsToRemove);
		}
		else
		{
			RightExitArrows.Empty();
		}
		break;

	case EOtherRoomPosition::LEFT:
		if (LeftCount > 0)
		{
			Exits = OtherRoom->RightExitArrows;
			for (int i = 0; i < LeftExitArrows.Num(); i++)
			{
				if (Exits.Num() > i)
				{
					if (LeftExitArrows[i]->GetComponentLocation().Y != Exits[i]->GetComponentLocation().Y)
					{
						ExitsToRemove.Add(LeftExitArrows[i]);
					}
				}
				else ExitsToRemove.Add(LeftExitArrows[i]);
			}
			RemoveRange(LeftExitArrows, ExitsToRemove);
			RemoveRange(OtherRoom->RightExitArrows, ExitsToRemove);
		}
		else
		{
			LeftExitArrows.Empty();
		}
		break;
	}

}

void ARoom::RemoveRange(TArray<UArrowComponent*>& FromArray, TArray<UArrowComponent*> RangeToDelete)
{
	for (auto Exit : RangeToDelete)
	{
		FromArray.Remove(Exit);
		//Exit->DestroyComponent();
	}
}

void ARoom::OnOverlapBeginUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)//(OtherComp->ComponentHasTag("Room") && OtherComp!=UpCollision)
	{
		UpCount++;
		UpRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("%s overlapping with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}
}

void ARoom::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)//(OtherComp->ComponentHasTag("Room") && OtherComp!=BottomCollision)
	{
		BottomCount++;
		BottomRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("%s overlapping with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}
}

void ARoom::OnOverlapBeginRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)//(OtherComp->ComponentHasTag("Room") && OtherComp!=RightCollision)//OtherActor->ActorHasTag("Room"))
	{
		RightCount++;
		RightRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("%s overlapping with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}
}

void ARoom::OnOverlapBeginLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Room") && OtherActor != this)//(OtherComp->ComponentHasTag("Room") && OtherComp!=LeftCollision)
	{
		LeftCount++;
		LeftRoom = Cast<ARoom>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("%s overlapping with %s"), *OverlappedComp->GetOwner()->GetName(), *OtherActor->GetName());
	}
}
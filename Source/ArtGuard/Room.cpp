// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Wall.h"
#include "ArtGuardGameMode.h"
#include "Area.h"

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
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoom::CreateWalls()
{
	if (!UpExit)
	{
		UpWalls.Add(CreateWall(FVector(Location.X, Location.Y + Height * 100 / 2, 800), FVector(Width, 1, 15)));
	}
	else
	{
		float Left = UpExit->Location.X - UpExit->Width * 100 / 2;
		float Right = UpExit->Location.X + UpExit->Width * 100 / 2;

		float LeftWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2, 0, 0), FVector(Left, 0, 0));
		float RightWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2, 0, 0), FVector(Right, 0, 0));

		UpWalls.Add(CreateWall(FVector((Left + (Location.X - Width * 100 / 2)) / 2, Location.Y + Height * 100 / 2, 800), FVector(LeftWallWidth / 100, 1, 15))); //15
		UpWalls.Add(CreateWall(FVector((Right + (Location.X + Width * 100 / 2)) / 2, Location.Y + Height * 100 / 2, 800), FVector(RightWallWidth / 100, 1, 15))); //15
	}
	CreateFrames(UpWalls, FRotator(0, 180, 0), true, -50);

	if (!DownExit)
	{
		DownWalls.Add(CreateWall(FVector(Location.X, Location.Y - Height * 100 / 2, 800), FVector(Width, 1, 15)));
	}
	else
	{
		float Left = DownExit->Location.X - DownExit->Width * 100 / 2;
		float Right = DownExit->Location.X + DownExit->Width * 100 / 2;

		float LeftWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2, 0, 0), FVector(Left, 0, 0));
		float RightWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2, 0, 0), FVector(Right, 0, 0));

		DownWalls.Add(CreateWall(FVector((Left + (Location.X - Width * 100 / 2)) / 2, Location.Y - Height * 100 / 2, 800), FVector(LeftWallWidth / 100, 1, 15)));
		DownWalls.Add(CreateWall(FVector((Right + (Location.X + Width * 100 / 2)) / 2, Location.Y - Height * 100 / 2, 800), FVector(RightWallWidth / 100, 1, 15)));
	}
	CreateFrames(DownWalls, FRotator(0), true, 50);

	if (!LeftExit)
	{
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, Location.Y, 800), FVector(1, Height, 15)));
	}
	else
	{
		float Up = LeftExit->Location.Y - LeftExit->Height * 100 / 2;
		float Down = LeftExit->Location.Y + LeftExit->Height * 100 / 2;

		float UpWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2, 0, 0), FVector(Up, 0, 0));
		float DownWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2, 0, 0), FVector(Down, 0, 0));

		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, (Up + (Location.Y - Height * 100 / 2)) / 2, 800), FVector(1, UpWallHeight / 100, 15)));
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, (Down + (Location.Y + Height * 100 / 2)) / 2, 800), FVector(1, DownWallHeight / 100, 15)));
	}
	CreateFrames(LeftWalls, FRotator(0, -90, 0), false, 50);

	if (!RightExit)
	{
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, Location.Y, 800), FVector(1, Height, 15)));
	}
	else
	{
		float Up = RightExit->Location.Y - RightExit->Height * 100 / 2;
		float Down = RightExit->Location.Y + RightExit->Height * 100 / 2;

		float UpWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2, 0, 0), FVector(Up, 0, 0));
		float DownWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2, 0, 0), FVector(Down, 0, 0));

		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, (Up + (Location.Y - Height * 100 / 2)) / 2, 800), FVector(1, UpWallHeight / 100, 15)));
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, (Down + (Location.Y + Height * 100 / 2)) / 2, 800), FVector(1, DownWallHeight / 100, 15)));
	}
	CreateFrames(RightWalls, FRotator(0, 90, 0), false, -50);
}

AWall* ARoom::CreateWall(FVector Location, FVector Scale, FRotator Rotation)
{
	auto Wall = GetWorld()->SpawnActor<AWall>(Wall_BP);
	Wall->SetActorLocation(Location);
	Wall->SetActorScale3D(Scale);
	Wall->SetActorRotation(Rotation);
	Wall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
	return Wall;
}

void ARoom::CreateFrames(TArray<AWall*> Walls, FRotator Rotation, bool IsHorizontal, int FrameOffset)
{
	for (auto Wall : Walls)
	{
		if (IsHorizontal)
		{
			int Width = Wall->GetActorScale3D().X;
			FVector Start = FVector(Wall->GetActorLocation().X - Width * 100 / 2, Wall->GetActorLocation().Y, 300);
			int NumberOfFrames = FMath::CeilToInt(Width / 5);
			for (int i = 0; i < NumberOfFrames; i++)
			{
				if (NumberOfFrames == 1)
					Wall->SpawnFrame(FVector(Wall->GetActorLocation().X, Start.Y + FrameOffset, Start.Z), Rotation);
				else
					Wall->SpawnFrame(FVector(Start.X + Width * 100 / NumberOfFrames / 2 + (Width * 100 / NumberOfFrames) * i, Start.Y + FrameOffset, Start.Z), Rotation);
			}
		}
		else
		{
			int Width = Wall->GetActorScale3D().Y;
			FVector Start = FVector(Wall->GetActorLocation().X, Wall->GetActorLocation().Y - Width * 100 / 2, 300);
			int NumberOfFrames = Width / 5;
			for (int i = 0; i < NumberOfFrames; i++)
			{
				if (NumberOfFrames == 1)
					Wall->SpawnFrame(FVector(Start.X + FrameOffset, Wall->GetActorLocation().Y, Start.Z), Rotation);
				else
					Wall->SpawnFrame(FVector(Start.X + FrameOffset, Start.Y + Width * 100 / NumberOfFrames / 2 + (Width * 100 / NumberOfFrames) * i, Start.Z), Rotation);
			}
		}
	}
}

void ARoom::CreateDecorWalls()
{
	if (FMath::RandBool())
	{
		int RandomRotation = PossibleRotationAnglesForDecorWalls[FMath::RandRange(0, PossibleRotationAnglesForDecorWalls.Num() - 1)];
		FRotator Rotation(0, RandomRotation, 0);
		FRotator OppositeRotation(0,180,0);
		FVector WallLocation;

		float LocationOffset = FMath::RandRange(5,10);//(4, 7);
		auto UpWall = CreateWall(FVector(Location.X, Location.Y + Height * 100 / LocationOffset, 800), FVector(Width / LocationOffset, 1, 15));
		WallLocation=UpWall->GetActorLocation();
		UpWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y+50, 300), UpWall->GetActorRotation());
		UpWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y-50, 300), UpWall->GetActorRotation() + OppositeRotation);
		UpWall->SetActorRotation(Rotation);
		
		auto DownWall = CreateWall(FVector(Location.X, Location.Y - Height * 100 / LocationOffset, 800), FVector(Width / LocationOffset, 1, 15));
		WallLocation=DownWall->GetActorLocation();
		DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y+50, 300), DownWall->GetActorRotation());
		DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y-50, 300), DownWall->GetActorRotation() + OppositeRotation);
		DownWall->SetActorRotation(Rotation + FRotator(0,180,0));

		auto LeftWall = CreateWall(FVector(Location.X - Width * 100 / LocationOffset, Location.Y, 800), FVector(Height / LocationOffset, 1, 15));
		WallLocation=LeftWall->GetActorLocation();
		LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y+50, 300), LeftWall->GetActorRotation());
		LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y-50, 300), LeftWall->GetActorRotation() + OppositeRotation);
		LeftWall->SetActorRotation(Rotation + FRotator(0,-90, 0));

		auto RightWall = CreateWall(FVector(Location.X + Width * 100 / LocationOffset, Location.Y, 800), FVector(Height / LocationOffset, 1, 15));
		WallLocation=RightWall->GetActorLocation();
		RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y+50, 300), RightWall->GetActorRotation());
		RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y-50, 300), RightWall->GetActorRotation() + OppositeRotation);
		RightWall->SetActorRotation(Rotation + FRotator(0,90, 0));
	}
}

void ARoom::SetFloor(UStaticMeshComponent* FloorToSet)
{
	Floor = FloorToSet;
}


void ARoom::SetGameMode()
{
	GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
}
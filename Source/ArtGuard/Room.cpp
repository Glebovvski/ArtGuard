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
	PrimaryActorTick.bCanEverTick = false;
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
			(Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode()))->TotalPictures+=NumberOfFrames;
			for (int i = 0; i < NumberOfFrames; i++)
			{
				if (NumberOfFrames == 1)
					Wall->SpawnFrame(FVector(Wall->GetActorLocation().X, Start.Y + FrameOffset, Start.Z), Rotation, true);
				else
					Wall->SpawnFrame(FVector(Start.X + Width * 100 / NumberOfFrames / 2 + (Width * 100 / NumberOfFrames) * i, Start.Y + FrameOffset, Start.Z), Rotation, true);
			}
		}
		else
		{
			int Width = Wall->GetActorScale3D().Y;
			FVector Start = FVector(Wall->GetActorLocation().X, Wall->GetActorLocation().Y - Width * 100 / 2, 300);
			int NumberOfFrames = FMath::CeilToInt(Width / 5);
			(Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode()))->TotalPictures+=NumberOfFrames;
			for (int i = 0; i < NumberOfFrames; i++)
			{
				if (NumberOfFrames == 1)
					Wall->SpawnFrame(FVector(Start.X + FrameOffset, Wall->GetActorLocation().Y, Start.Z), Rotation, true);
				else
					Wall->SpawnFrame(FVector(Start.X + FrameOffset, Start.Y + Width * 100 / NumberOfFrames / 2 + (Width * 100 / NumberOfFrames) * i, Start.Z), Rotation, true);
			}
		}
	}
}


void ARoom::CreateDecorWalls()
{
	auto GM = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
	if (FMath::RandBool())
	{
		if (Width > 8 && Height > 8)
		{
			float SecondScale = 0.5;
			float FrameOffset = 25;

			int RandomRotation = PossibleRotationAnglesForDecorWalls[FMath::RandRange(0, PossibleRotationAnglesForDecorWalls.Num() - 1)];
			FRotator Rotation(0, RandomRotation, 0);
			FRotator OppositeRotation(0, 180, 0);
			FVector WallLocation;

			float LocationOffset = FMath::RandRange(5, 8);
			AWall* UpWall;
			if (Rotation == FRotator(0, 90, 0) && Width / Height >= 2)
				UpWall = CreateWall(FVector(Location.X, Location.Y + Height * 100 / (LocationOffset - 1), 800), FVector(Height / LocationOffset, SecondScale, 15));
			else
				UpWall = CreateWall(FVector(Location.X, Location.Y + Height * 100 / (LocationOffset - 1), 800), FVector(Width / LocationOffset, SecondScale, 15));
			WallLocation = UpWall->GetActorLocation();
			UpWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), UpWall->GetActorRotation() + OppositeRotation, false);
			UpWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), UpWall->GetActorRotation(), false);
			UpWall->SetActorRotation(Rotation);
			GM->TotalPictures+=2;

			AWall* DownWall;
			if (Rotation == FRotator(0, 90, 0) && Width / Height >= 2)
				DownWall = CreateWall(FVector(Location.X, Location.Y - Height * 100 / (LocationOffset - 1), 800), FVector(Height / LocationOffset, SecondScale, 15));
			else
				DownWall = CreateWall(FVector(Location.X, Location.Y - Height * 100 / (LocationOffset - 1), 800), FVector(Width / LocationOffset, SecondScale, 15));
			WallLocation = DownWall->GetActorLocation();
			DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), DownWall->GetActorRotation() + OppositeRotation, false);
			DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), DownWall->GetActorRotation(), false);
			DownWall->SetActorRotation(Rotation + FRotator(0, 180, 0));
			GM->TotalPictures+=2;

			AWall* LeftWall;
			if (Rotation == FRotator(0, 90, 0) && Height / Width >= 2)
				LeftWall = CreateWall(FVector(Location.X - Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Width / LocationOffset, SecondScale, 15));
			else
				LeftWall = CreateWall(FVector(Location.X - Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Height / LocationOffset, SecondScale, 15));
			WallLocation = LeftWall->GetActorLocation();
			LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), LeftWall->GetActorRotation(), false);
			LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), LeftWall->GetActorRotation() + OppositeRotation, false);
			LeftWall->SetActorRotation(Rotation + FRotator(0, -90, 0));
			GM->TotalPictures+=2;

			AWall* RightWall;
			if (Rotation == FRotator(0, 90, 0) && Height / Width >= 2)
				RightWall = CreateWall(FVector(Location.X + Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Width / LocationOffset, SecondScale, 15));
			else
				RightWall = CreateWall(FVector(Location.X + Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Height / LocationOffset, SecondScale, 15));
			WallLocation = RightWall->GetActorLocation();
			RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), RightWall->GetActorRotation(), false);
			RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), RightWall->GetActorRotation() + OppositeRotation, false);
			RightWall->SetActorRotation(Rotation + FRotator(0, 90, 0));
			GM->TotalPictures+=2;
		}
	}
	else
	{
		auto Statue = GetWorld()->SpawnActor<AActor>(BP_BigStatue);
		Statue->SetActorLocation(GetActorLocation());
	}
}

void ARoom::CreateProps()
{
	int DistanceFromWalls = 5;
	if (Width > Height)
	{
		int NumberOfBenches = Width / 5;
		int offset = (Width / NumberOfBenches) * 100;
		NumberOfBenches--;
		if (NumberOfBenches > 1)
		{
			for (int i = 0; i < NumberOfBenches; i++)
			{
				FVector LeftLocation = FVector(Location.X - Width * 100 / 2 + (i + 1) * offset, Location.Y - Height * 100 / DistanceFromWalls, 50);
				FVector RightLocation = FVector(Location.X - Width * 100 / 2 + (i + 1) * offset, Location.Y + Height * 100 / DistanceFromWalls, 50);
				if (DownExit)
				{
					if (NumberOfBenches % 2 != 0)
					{
						if (i != NumberOfBenches / 2)
							auto Bench = CreateProp(LeftLocation);
					}
					else
					{
						auto Bench = CreateProp(LeftLocation);
					}
				}
				else
				{
					CreateProp(LeftLocation);
				}
				if (UpExit)
				{
					if (NumberOfBenches % 2 != 0)
					{
						if (i != NumberOfBenches / 2)
							auto Bench = CreateProp(RightLocation);
					}
					else
					{
						auto Bench = CreateProp(RightLocation);
					}
				}
				else
				{
					CreateProp(RightLocation);
				}
			}
		}
	}
	else
	{
		int NumberOfBenches = Height / 5;
		int offset = (Height / NumberOfBenches) * 100;
		NumberOfBenches--;
		if (NumberOfBenches > 1)
		{
			for (int i = 0; i < NumberOfBenches; i++)
			{
				FVector LeftLocation = FVector(Location.X - Width * 100 / DistanceFromWalls, Location.Y - Height * 100 / 2 + (i + 1) * offset, 50);
				FVector RightLocation = FVector(Location.X + Width * 100 / DistanceFromWalls, Location.Y - Height * 100 / 2 + (i + 1) * offset, 50);
				if (LeftExit)
				{
					if (NumberOfBenches % 2 != 0)
					{
						if (i != NumberOfBenches / 2)
							auto Bench = CreateProp(LeftLocation, FRotator(0, 90, 0));
					}
					else
					{
						auto Bench = CreateProp(LeftLocation, FRotator(0, 90, 0));
					}
				}
				else
				{
					CreateProp(LeftLocation, FRotator(0, 90, 0));
				}

				if (RightExit)
				{
					if (NumberOfBenches % 2 != 0)
					{
						if (i != NumberOfBenches / 2)
							auto Bench = CreateProp(RightLocation, FRotator(0, 90, 0));
					}
					else
					{
						auto Bench = CreateProp(RightLocation, FRotator(0, 90, 0));
					}
				}
				else
				{
					CreateProp(RightLocation, FRotator(0, 90, 0));
				}
			}
		}
	}
}


AActor* ARoom::CreateProp(FVector Location, FRotator Rotation)
{
	FTransform BenchTransform = FTransform(
		Rotation,
		Location
	);

	auto Bench = UGameplayStatics::BeginSpawningActorFromClass(this, BP_Bench, BenchTransform);//GetWorld()->SpawnActor<AActor>(BP_Bench);
	if (Bench)
	{
		UGameplayStatics::FinishSpawningActor(Bench, BenchTransform);
	}
	return Bench;
}

bool ARoom::IsInExitLine(FVector Start, FVector End, bool DebugDraw)
{
	TArray<FHitResult> OutHits;
	FCollisionShape BoxCollision = FCollisionShape::MakeBox(FVector(180, 180, 180));
	//FBox Box = FBox(FVector(180,180,180));
	if (DebugDraw)
	{
		DrawDebugBox(GetWorld(), Start, FVector(180), FColor::Red, true, 1000);
		DrawDebugBox(GetWorld(), End, FVector(180), FColor::Red, true, 1000);
	}

	bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_WorldDynamic, BoxCollision);
	//bool IsHit = GetWorld()->SweepMultiByObjectType(OutHits, Start, End, FQuat::Identity, ECC_WorldDynamic, BoxCollision);
	if (IsHit)
	{
		for (auto OutHit : OutHits)
		{
			if (OutHit.GetActor()->ActorHasTag("Props"))
			{
				return true;
			}
		}
	}
	return false;
}

void ARoom::SetFloor(UStaticMeshComponent* FloorToSet)
{
	Floor = FloorToSet;
}


void ARoom::SetGameMode()
{
	GameMode = Cast<AArtGuardGameMode>(GetWorld()->GetAuthGameMode());
}

void ARoom::SpawnPuddle(int& Iterator)
{
	AArea* Exit = CreatedExits[FMath::RandRange(0, CreatedExits.Num() - 1)];
	//for (AArea* Exit : CreatedExits)
	//{
	if (FMath::RandRange(0, 1))
	{
		FVector Distance = (Location + Exit->GetActorLocation()) / 2;
		FTransform PuddleTransform = FTransform(
			FRotator::ZeroRotator,
			Distance + FVector(0, 0, 50)//RandomExit->GetActorLocation() + FVector(700,700,50)//z maybe 50
		);

		auto Puddle = UGameplayStatics::BeginSpawningActorFromClass(this, BP_PuddleGenerator, PuddleTransform);//GetWorld()->SpawnActor<AActor>(BP_Bench);
		if (Puddle)
		{
			UGameplayStatics::FinishSpawningActor(Puddle, PuddleTransform);
		}
		Iterator--;
	}
	//}
}

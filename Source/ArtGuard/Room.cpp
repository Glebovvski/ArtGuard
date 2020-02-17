// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Wall.h"
#include "ArtGuardGameMode.h"
#include "Exit.h"
#include "Space.h"
#include "Area.h"

// Sets default values
ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FTransform BoxTransform = FTransform(
		FRotator::ZeroRotator,
		FVector(0, 0, 0),
		FVector(1, 1, 1)
	);
	Box = CreateDefaultSubobject<UStaticMeshComponent>(FName("Box"));
	Box->SetWorldTransform(BoxTransform);
	SetRootComponent(Box);

	//RoomDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("DetectionSphere"));
	//FTransform SphereTransform = FTransform(
	//	FRotator::ZeroRotator,
	//	FVector(0, 0, 0), //x=-105
	//	FVector(10) //x=0.5
	//);
	//RoomDetectionSphere->SetRelativeTransform(SphereTransform);

	//RoomDetectionSphere->AttachToComponent(Box, FAttachmentTransformRules::KeepWorldTransform);

	//RoomDetectionSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverlaps(true);
	bGenerateOverlapEventsDuringLevelStreaming = true;
	
	//RoomDetectionSphere->SetSphereRadius((FMath::Sqrt(Width * Width + Height * Height) / 2)*10);

	//UE_LOG(LogTemp, Warning, TEXT("PARENT AREA: %s"), *(GetParentActor() ? GetParentActor()->GetName() : TEXT("NULL")));
}

void ARoom::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateOverlaps(true);
	bGenerateOverlapEventsDuringLevelStreaming = true;
}

void ARoom::DestroyRoom()
{
	for (auto UpWall : UpWalls)
	{
		UpWall->DestroyWall();
		UpWall->Destroy(false, true);
	}
	for (auto LeftWall : LeftWalls)
	{
		LeftWall->DestroyWall();
		LeftWall->Destroy(false, true);
	}
	for (auto DownWall : DownWalls)
	{
		DownWall->DestroyWall();
		DownWall->Destroy(false, true);
	}
	for (auto RightWall : RightWalls)
	{
		RightWall->DestroyWall();
		RightWall->Destroy(false, true);
	}
	for (auto DecorWall : DecorWalls)
	{
		DecorWall->DestroyWall();
		DecorWall->Destroy(false, true);
	}
	for (auto Bench : Benches)
	{
		Bench->Destroy();
	}
}


// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoom::CreateWalls()
{
	SortExits();

	float WallOffset = 50;

	if (UpExits.Num() == 0)//(!UpExit)
	{
		UpWalls.Add(CreateWall(FVector(Location.X, Location.Y + Height * 100 / 2, 800), FVector(Width, 1, 15)));
	}
	else if (UpExits.Num() == 1)
	{
		float Left = UpExits[0]->Location.X - UpExits[0]->Width * 100 / 2 - WallOffset;
		float Right = UpExits[0]->Location.X + UpExits[0]->Width * 100 / 2 + WallOffset;

		float LeftWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2, 0, 0), FVector(Left, 0, 0));
		float RightWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2, 0, 0), FVector(Right, 0, 0));

		UpWalls.Add(CreateWall(FVector((Left + (Location.X - Width * 100 / 2)) / 2, Location.Y + Height * 100 / 2, 800), FVector(LeftWallWidth / 100, 1, 15))); //15
		UpWalls.Add(CreateWall(FVector((Right + (Location.X + Width * 100 / 2)) / 2, Location.Y + Height * 100 / 2, 800), FVector(RightWallWidth / 100, 1, 15))); //15
	}
	else //if(UpExits.Num() == 2)
	{
		ISpace* FirstExit = UpExits[0];
		ISpace* SecondExit = UpExits[1];

		//FIRST WALL
		float FirstWallX = ((Location.X - Width * 100 / 2) + (FirstExit->Location.X - FirstExit->Width * 100 / 2)) / 2;
		float FirstWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2 + WallOffset, 0, 0), FVector(FirstExit->Location.X - FirstExit->Width * 100 / 2 - WallOffset, 0, 0));
		UpWalls.Add(CreateWall(FVector(FirstWallX, Location.Y + Height * 100 / 2, 800), FVector(FirstWallWidth / 100, 1, 15)));

		//SECOND WALL
		float SecondWallX = ((FirstExit->Location.X - WallOffset + FirstExit->Width * 100 / 2) + (SecondExit->Location.X + WallOffset - SecondExit->Width * 100 / 2)) / 2;
		float SecondWallWidth = FVector::Distance(FVector(FirstExit->Location.X + FirstExit->Width * 100 / 2 + WallOffset, 0, 0), FVector(SecondExit->Location.X - SecondExit->Width * 100 / 2 - WallOffset, 0, 0));
		UpWalls.Add(CreateWall(FVector(SecondWallX, Location.Y + Height * 100 / 2, 800), FVector(SecondWallWidth / 100, 1, 15)));

		//THIRD WALL
		float ThirdWallX = ((Location.X + Width * 100 / 2) + (SecondExit->Location.X + SecondExit->Width * 100 / 2)) / 2;
		float ThirdWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2 - WallOffset, 0, 0), FVector(SecondExit->Location.X + SecondExit->Width * 100 / 2 + WallOffset, 0, 0));
		UpWalls.Add(CreateWall(FVector(ThirdWallX, Location.Y + Height * 100 / 2, 800), FVector(ThirdWallWidth / 100, 1, 15)));
	}
	CreateFrames(UpWalls, FRotator(0, 180, 0), true, -50);

	if (DownExits.Num() == 0)//(!DownExit)
	{
		DownWalls.Add(CreateWall(FVector(Location.X, Location.Y - Height * 100 / 2, 800), FVector(Width, 1, 15)));
	}
	else if (DownExits.Num() == 1)
	{
		float Left = DownExits[0]->Location.X - DownExits[0]->Width * 100 / 2 - WallOffset;
		float Right = DownExits[0]->Location.X + DownExits[0]->Width * 100 / 2 + WallOffset;

		float LeftWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2, 0, 0), FVector(Left, 0, 0));
		float RightWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2, 0, 0), FVector(Right, 0, 0));

		DownWalls.Add(CreateWall(FVector((Left + (Location.X - Width * 100 / 2)) / 2, Location.Y - Height * 100 / 2, 800), FVector(LeftWallWidth / 100, 1, 15)));
		DownWalls.Add(CreateWall(FVector((Right + (Location.X + Width * 100 / 2)) / 2, Location.Y - Height * 100 / 2, 800), FVector(RightWallWidth / 100, 1, 15)));
	}
	else //if (DownExits.Num() == 2)
	{
		ISpace* FirstExit = DownExits[0];
		ISpace* SecondExit = DownExits[1];

		//FIRST WALL
		float FirstWallX = ((Location.X - Width * 100 / 2) + (FirstExit->Location.X - FirstExit->Width * 100 / 2)) / 2;
		float FirstWallWidth = FVector::Distance(FVector(Location.X - Width * 100 / 2 + WallOffset, 0, 0), FVector(FirstExit->Location.X - FirstExit->Width * 100 / 2 - WallOffset, 0, 0)); //MINUS WALLOFFSET
		DownWalls.Add(CreateWall(FVector(FirstWallX, Location.Y - Height * 100 / 2, 800), FVector(FirstWallWidth / 100, 1, 15)));

		//SECOND WALL
		float SecondWallX = ((FirstExit->Location.X + FirstExit->Width * 100 / 2) + (SecondExit->Location.X - SecondExit->Width * 100 / 2)) / 2;
		float SecondWallWidth = FVector::Distance(FVector(FirstExit->Location.X + FirstExit->Width * 100 / 2 + WallOffset, 0, 0), FVector(SecondExit->Location.X - SecondExit->Width * 100 / 2 - WallOffset, 0, 0));
		DownWalls.Add(CreateWall(FVector(SecondWallX, Location.Y - Height * 100 / 2, 800), FVector(SecondWallWidth / 100, 1, 15)));

		//THIRD WALL
		float ThirdWallX = ((Location.X + Width * 100 / 2) + (SecondExit->Location.X + SecondExit->Width * 100 / 2)) / 2;
		float ThirdWallWidth = FVector::Distance(FVector(Location.X + Width * 100 / 2 - WallOffset, 0, 0), FVector(SecondExit->Location.X + SecondExit->Width * 100 / 2 + WallOffset, 0, 0));
		DownWalls.Add(CreateWall(FVector(ThirdWallX, Location.Y - Height * 100 / 2, 800), FVector(ThirdWallWidth / 100, 1, 15)));
	}
	CreateFrames(DownWalls, FRotator(0), true, 50);

	if (LeftExits.Num() == 0)//(!LeftExit)
	{
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, Location.Y, 800), FVector(1, Height, 15)));
	}
	else if (LeftExits.Num() == 1)
	{
		float Up = LeftExits[0]->Location.Y - LeftExits[0]->Height * 100 / 2 - WallOffset;
		float Down = LeftExits[0]->Location.Y + LeftExits[0]->Height * 100 / 2 + WallOffset;

		float UpWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2, 0, 0), FVector(Up, 0, 0));
		float DownWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2, 0, 0), FVector(Down, 0, 0));

		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, (Up + (Location.Y - Height * 100 / 2)) / 2, 800), FVector(1, UpWallHeight / 100, 15)));
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, (Down + (Location.Y + Height * 100 / 2)) / 2, 800), FVector(1, DownWallHeight / 100, 15)));
	}
	else
	{
		ISpace* FirstExit = LeftExits[0];
		ISpace* SecondExit = LeftExits[1];

		//FIRST WALL
		float FirstWallY = ((Location.Y - Height * 100 / 2) + (FirstExit->Location.Y - FirstExit->Height * 100 / 2)) / 2;
		float FirstWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2 + WallOffset, 0, 0), FVector(FirstExit->Location.Y - FirstExit->Height * 100 / 2 - WallOffset, 0, 0));
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, FirstWallY, 800), FVector(1, FirstWallHeight / 100, 15)));

		//SECOND WALL
		float SecondWallY = ((FirstExit->Location.Y + FirstExit->Height * 100 / 2) + (SecondExit->Location.Y - SecondExit->Height * 100 / 2)) / 2;
		float SecondWallHeight = FVector::Distance(FVector(FirstExit->Location.Y + FirstExit->Height * 100 / 2 + WallOffset, 0, 0), FVector(SecondExit->Location.Y - SecondExit->Height * 100 / 2 - WallOffset, 0, 0));
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, SecondWallY, 800), FVector(1, SecondWallHeight / 100, 15)));

		//THIRD WALL
		float ThirdWallY = ((Location.Y + Height * 100 / 2) + (SecondExit->Location.Y + SecondExit->Height * 100 / 2)) / 2;
		float ThirdWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2 - WallOffset, 0, 0), FVector(SecondExit->Location.Y + SecondExit->Height * 100 / 2 + WallOffset, 0, 0));
		LeftWalls.Add(CreateWall(FVector(Location.X - Width * 100 / 2, ThirdWallY, 800), FVector(1, ThirdWallHeight / 100, 15)));
	}
	CreateFrames(LeftWalls, FRotator(0, -90, 0), false, 50);

	if (RightExits.Num() == 0)//(!RightExit)
	{
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, Location.Y, 800), FVector(1, Height, 15)));
	}
	else if (RightExits.Num() == 1)
	{
		float Up = RightExits[0]->Location.Y - RightExits[0]->Height * 100 / 2 - WallOffset;
		float Down = RightExits[0]->Location.Y + RightExits[0]->Height * 100 / 2 + WallOffset;

		float UpWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2, 0, 0), FVector(Up, 0, 0));
		float DownWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2, 0, 0), FVector(Down, 0, 0));

		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, (Up + (Location.Y - Height * 100 / 2)) / 2, 800), FVector(1, UpWallHeight / 100, 15)));
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, (Down + (Location.Y + Height * 100 / 2)) / 2, 800), FVector(1, DownWallHeight / 100, 15)));
	}
	else
	{
		ISpace* FirstExit = RightExits[0];
		ISpace* SecondExit = RightExits[1];

		//FIRST WALL
		float FirstWallY = ((Location.Y - Height * 100 / 2) + (FirstExit->Location.Y - FirstExit->Height * 100 / 2)) / 2;
		float FirstWallHeight = FVector::Distance(FVector(Location.Y - Height * 100 / 2 + WallOffset, 0, 0), FVector(FirstExit->Location.Y - FirstExit->Height * 100 / 2 - WallOffset, 0, 0));
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, FirstWallY, 800), FVector(1, FirstWallHeight / 100, 15)));

		//SECOND WALL
		float SecondWallY = ((FirstExit->Location.Y + FirstExit->Height * 100 / 2) + (SecondExit->Location.Y - SecondExit->Height * 100 / 2)) / 2;
		float SecondWallHeight = FVector::Distance(FVector(FirstExit->Location.Y + FirstExit->Height * 100 / 2 + WallOffset, 0, 0), FVector(SecondExit->Location.Y - SecondExit->Height * 100 / 2 - WallOffset, 0, 0));
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, SecondWallY, 800), FVector(1, SecondWallHeight / 100, 15)));

		//THIRD WALL
		float ThirdWallY = ((Location.Y + Height * 100 / 2) + (SecondExit->Location.Y + SecondExit->Height * 100 / 2)) / 2;
		float ThirdWallHeight = FVector::Distance(FVector(Location.Y + Height * 100 / 2 - WallOffset, 0, 0), FVector(SecondExit->Location.Y + SecondExit->Height * 100 / 2 + WallOffset, 0, 0));
		RightWalls.Add(CreateWall(FVector(Location.X + Width * 100 / 2, ThirdWallY, 800), FVector(1, ThirdWallHeight / 100, 15)));
	}
	CreateFrames(RightWalls, FRotator(0, 90, 0), false, -50);
}

AWall* ARoom::CreateWall(FVector Location, FVector Scale, FRotator Rotation)
{
	return SpawnWall(Location, Scale, Rotation);
}

AWall* ARoom::CreateWall(FVector Location, FVector Scale)
{
	return SpawnWall(Location, Scale, FRotator::ZeroRotator);
}

AWall* ARoom::SpawnWall(FVector Location, FVector Scale, FRotator Rotation)
{
	FTransform WallTransform = FTransform(
		Rotation,
		Location
	);

	auto Wall = Cast<AWall>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), Wall_BP, WallTransform));
	if (Wall)
	{
		UGameplayStatics::FinishSpawningActor(Wall, WallTransform);
		Wall->SetActorScale3D(Scale);
		Wall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
	}

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
			GameMode->TotalPictures += NumberOfFrames;
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
			GameMode->TotalPictures += NumberOfFrames;
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
	if (!IsBiggestRoom)
	{
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
				UpWall->Wall->SetMaterial(0, DecorWallMat);
				GameMode->TotalPictures += 2;
				DecorWalls.Add(UpWall);
				
				AWall* DownWall;
				if (Rotation == FRotator(0, 90, 0) && Width / Height >= 2)
					DownWall = CreateWall(FVector(Location.X, Location.Y - Height * 100 / (LocationOffset - 1), 800), FVector(Height / LocationOffset, SecondScale, 15));
				else
					DownWall = CreateWall(FVector(Location.X, Location.Y - Height * 100 / (LocationOffset - 1), 800), FVector(Width / LocationOffset, SecondScale, 15));
				DownWall->Wall->SetMaterial(0, DecorWallMat);
				WallLocation = DownWall->GetActorLocation();
				DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), DownWall->GetActorRotation() + OppositeRotation, false);
				DownWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), DownWall->GetActorRotation(), false);
				DownWall->SetActorRotation(Rotation + FRotator(0, 180, 0));
				GameMode->TotalPictures += 2;
				DecorWalls.Add(DownWall);

				AWall* LeftWall;
				if (Rotation == FRotator(0, 90, 0) && Height / Width >= 2)
					LeftWall = CreateWall(FVector(Location.X - Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Width / LocationOffset, SecondScale, 15));
				else
					LeftWall = CreateWall(FVector(Location.X - Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Height / LocationOffset, SecondScale, 15));
				LeftWall->Wall->SetMaterial(0, DecorWallMat);
				WallLocation = LeftWall->GetActorLocation();
				LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), LeftWall->GetActorRotation(), false);
				LeftWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), LeftWall->GetActorRotation() + OppositeRotation, false);
				LeftWall->SetActorRotation(Rotation + FRotator(0, -90, 0));
				GameMode->TotalPictures += 2;
				DecorWalls.Add(LeftWall);

				AWall* RightWall;
				if (Rotation == FRotator(0, 90, 0) && Height / Width >= 2)
					RightWall = CreateWall(FVector(Location.X + Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Width / LocationOffset, SecondScale, 15));
				else
					RightWall = CreateWall(FVector(Location.X + Width * 100 / (LocationOffset - 1), Location.Y, 800), FVector(Height / LocationOffset, SecondScale, 15));
				RightWall->Wall->SetMaterial(0, DecorWallMat);
				WallLocation = RightWall->GetActorLocation();
				RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y + FrameOffset, 300), RightWall->GetActorRotation(), false);
				RightWall->SpawnFrame(FVector(WallLocation.X, WallLocation.Y - FrameOffset, 300), RightWall->GetActorRotation() + OppositeRotation, false);
				RightWall->SetActorRotation(Rotation + FRotator(0, 90, 0));
				GameMode->TotalPictures += 2;
				DecorWalls.Add(RightWall);
			}
		}
	}
	//else
	//{
	//		auto Statue = GetWorld()->SpawnActor<AActor>(BP_BigStatue);
	//		Statue->SetActorLocation(GetActorLocation());
	//}
}

void ARoom::CreateProps()
{
	float Z = 55;
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
				FVector LeftLocation = FVector(Location.X - Width * 100 / 2 + (i + 1) * offset, Location.Y - Height * 100 / DistanceFromWalls, Z);
				FVector RightLocation = FVector(Location.X - Width * 100 / 2 + (i + 1) * offset, Location.Y + Height * 100 / DistanceFromWalls, Z);
				if (DownExits.Num() > 0)//(DownExit)
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
				if (UpExits.Num() > 0)//(UpExit)
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
				FVector LeftLocation = FVector(Location.X - Width * 100 / DistanceFromWalls, Location.Y - Height * 100 / 2 + (i + 1) * offset, Z);
				FVector RightLocation = FVector(Location.X + Width * 100 / DistanceFromWalls, Location.Y - Height * 100 / 2 + (i + 1) * offset, Z);
				if (LeftExits.Num() > 0)//(LeftExit)
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

				if (RightExits.Num() > 0)//(RightExit)
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

	auto Bench = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_Bench, BenchTransform);//GetWorld()->SpawnActor<AActor>(BP_Bench);
	if (Bench)
	{
		UGameplayStatics::FinishSpawningActor(Bench, BenchTransform);
	}
	Benches.Add(Bench);
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

void ARoom::SortExits()
{
	if (UpExits.Num() > 1)
	{
		if (UpExits[0]->Location.X > UpExits[1]->Location.X)
			Swap(UpExits);
	}
	if (DownExits.Num() > 1)
	{
		if (DownExits[0]->Location.X > DownExits[1]->Location.X)
			Swap(DownExits);
	}
	if (RightExits.Num() > 1)
	{
		if (RightExits[0]->Location.Y > RightExits[1]->Location.Y)
			Swap(RightExits);
	}
	if (LeftExits.Num() > 1)
	{
		if (LeftExits[0]->Location.Y > LeftExits[1]->Location.Y)
			Swap(LeftExits);
	}
}

void ARoom::Swap(TArray<ISpace*>& Exits)
{
	ISpace* temp = Exits[0];
	Exits[0] = Exits[1];
	Exits[1] = temp;
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
	if (CreatedExits.Num() > 0)
	{
		auto Exit = Cast<AArea>(CreatedExits[FMath::RandRange(0, CreatedExits.Num() - 1)]);
		if (FMath::RandRange(0, 1) && Exit)
		{
			FVector Distance = (Location + Exit->GetActorLocation()) / 2;
			FTransform PuddleTransform = FTransform(
				FRotator::ZeroRotator,
				Distance + FVector(0, 0, 50)//RandomExit->GetActorLocation() + FVector(700,700,50)//z maybe 50
			);

			auto Puddle = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BP_PuddleGenerator, PuddleTransform);//GetWorld()->SpawnActor<AActor>(BP_Bench);
			if (Puddle)
			{
				UGameplayStatics::FinishSpawningActor(Puddle, PuddleTransform);
			}
		}
		Iterator--;
	}
}

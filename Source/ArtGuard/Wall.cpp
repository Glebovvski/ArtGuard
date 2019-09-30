// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "GameFramework/Actor.h"
#include "Frame.h"
#include "Components/ArrowComponent.h"
#include "Room.h"

// Sets default values
AWall::AWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AWall::SetWallProperties(UArrowComponent* ArrowToSet, UStaticMeshComponent* WallToSet)
{
	Arrow = ArrowToSet;
	Wall = WallToSet;
}


void AWall::CreateFrames()
{
	/*
	float Width = GetActorScale3D().X > GetActorScale3D().Y ? GetActorScale3D().X : GetActorScale3D().Y;
	FVector Location = GetActorLocation();
	ARoom* Room = Cast<ARoom>(GetAttachParentActor());
	FVector RoomLocation = Room->GetActorLocation();
	//Figure out wall position
	if (RoomLocation.Y == Location.Y)
	{
		if (FVector(RoomLocation.X + Room->Width * 100 / 2, Location.Y, 0) == Location)
		{
			//Right Wall
			SpawnFrame(FVector(Location.X, Location.Y, 300));
		}
		else if (RoomLocation.X - Room->Width * 100 / 2 == Location.X)
		{
			//Left Wall
			SpawnFrame(FVector(Location.X, Location.Y, 300));
		}
	}
	else if (RoomLocation.X == Location.X)
	{
		if (RoomLocation.Y + Room->Height * 100 / 2 == Location.Y)
		{
			//Up Room
			SpawnFrame(FVector(Location.X, Location.Y, 300));
		}
		else if (RoomLocation.Y - Room->Height * 100 / 2 == Location.Y)
		{
			//Down Room
			SpawnFrame(FVector(Location.X, Location.Y, 300));
		}
	}

	//int NumberOfFrames = Width / 5;
	//for (int i = 0; i < NumberOfFrames; i++)
	//{
	//	FVector Location = FVector( GetActorLocation().X + NumberOfFrames * 100 / 2 + i * 100 + 50, GetActorLocation().Y, 300);
	//	SpawnFrame(Location);
	//}
	*/
}


AFrame* AWall::SpawnFrame(FVector Location, FRotator Rotation)
{
	auto Frame = GetWorld()->SpawnActor<AFrame>(Frame_BP);
	//Frame->SetActorTransform(Arrow->GetComponentTransform());
	//FRotator ArrowRotation = Arrow->GetComponentRotation();
	Frame->AddActorWorldRotation(Rotation);
	Frame->SetActorLocation(Location);
	Frame->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
	Frame->SpawnPicture();
	return Frame;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


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

void AWall::SetWallProperties(UStaticMeshComponent* WallToSet)
{
	Wall = WallToSet;
}


AFrame* AWall::SpawnFrame(FVector Location, FRotator Rotation, bool IsFenceVisible)
{
	auto Frame = GetWorld()->SpawnActor<AFrame>(Frame_BP);
	//Frame->SetActorTransform(Arrow->GetComponentTransform());
	//FRotator ArrowRotation = Arrow->GetComponentRotation();
	Frame->AddActorWorldRotation(Rotation);
	Frame->SetActorLocation(Location);
	Frame->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
	Frame->SpawnPicture(IsFenceVisible);
	return Frame;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();

}
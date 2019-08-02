// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"
#include "GameFramework/Actor.h"
#include "Frame.h"
#include "Components/ArrowComponent.h"

// Sets default values
AWall::AWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWall::SetWallProperties(UArrowComponent* ArrowToSet, UStaticMeshComponent* WallToSet)
{
	Arrow = ArrowToSet;
	Wall = WallToSet;
}

AFrame* AWall::SpawnFrame()
{
	if (Arrow)
	{
		auto Frame = GetWorld()->SpawnActor<AFrame>(Frame_BP);
		Frame->SetActorTransform(Arrow->GetComponentTransform());
		FRotator ArrowRotation = Arrow->GetComponentRotation();
		Frame->AddActorLocalRotation(FRotator(ArrowRotation.Pitch, -90, ArrowRotation.Roll));
		Frame->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
		return Frame;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ARROW ON WALL"));
		return nullptr;
	}
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


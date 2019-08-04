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

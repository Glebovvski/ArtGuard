// Fill out your copyright notice in the Description page of Project Settings.

#include "Flashlight.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
AFlashlight::AFlashlight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void AFlashlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);//Tick(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool AFlashlight::CheckFlashlightCollision()
{
	//FHitResult OutHit;
	//bool DidTrace = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(), GetActorForwardVector(), ECC_WorldDynamic);
	//
	//if(DidTrace)
	//{
	//	if(OutHit)
	//}
	return true;
}


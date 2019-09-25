// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Flashlight.generated.h"


UCLASS()
class ARTGUARD_API AFlashlight : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AFlashlight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CheckFlashlightCollision();
};

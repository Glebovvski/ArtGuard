// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Space.h"
#include "Exit.generated.h"

UCLASS()
class ARTGUARD_API AExit : public AActor, public ISpace
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	/*UPROPERTY(VisibleInstanceOnly)
		float Height;
	UPROPERTY(VisibleInstanceOnly)
		float Width;

	FVector Location;*/

};

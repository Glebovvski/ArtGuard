// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

class UArrowComponent;
class AFrame;

UCLASS()
class ARTGUARD_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	UArrowComponent* Arrow;
	UStaticMeshComponent* Wall;

	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetWallProperties(UArrowComponent* ArrowToSet, UStaticMeshComponent* WallToSet);

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AFrame> Frame_BP;

	UFUNCTION(BlueprintCallable, Category="Spawn")
	AFrame* SpawnFrame(FVector Location, FRotator Rotation, bool IsFenceVisible);

	void CreateFrames();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGuardGameMode.h"
#include "Room.generated.h"

class AWall;
class UArrowComponent;
class AExit;
class AArtGuardGameMode;
class UBoxComponent;

UCLASS()
class ARTGUARD_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float Width;
	float Height;
	FVector Location;

	AArea* RightExit;
	AArea* LeftExit;
	AArea* UpExit;
	AArea* DownExit;

	void CreateWalls();
	TArray<AWall*> LeftWalls;
	TArray<AWall*> RightWalls;
	TArray<AWall*> UpWalls;
	TArray<AWall*> DownWalls;

	void CreateDecorWalls();

	UPROPERTY(EditDefaultsOnly, Category="Room")
	UStaticMeshComponent* Floor;

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetFloor(UStaticMeshComponent* FloorToSet);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetGameMode();

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AWall> Wall_BP;
private:
	AArtGuardGameMode* GameMode;
	AWall* CreateWall(FVector Location, FVector Scale, FRotator Rotation=FRotator::ZeroRotator);
	ARoom* UpRoom;
	ARoom* BottomRoom;
	ARoom* RightRoom;
	ARoom* LeftRoom;

	TArray<int> PossibleRotationAnglesForDecorWalls = TArray<int>{ 0,90,180 };

	void CreateFrames(TArray<AWall*> Walls, FRotator Rotation, bool IsHorizontal, int FrameOffset);
};

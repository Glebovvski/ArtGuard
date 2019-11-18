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
	TArray<AArea*> CreatedExits;

	void CreateWalls();
	TArray<AWall*> LeftWalls;
	TArray<AWall*> RightWalls;
	TArray<AWall*> UpWalls;
	TArray<AWall*> DownWalls;

	void CreateDecorWalls();
	void CreateProps();
	UPROPERTY(EditDefaultsOnly, Category="Props")
	TSubclassOf<AActor> BP_Bench;

	UPROPERTY(EditDefaultsOnly, Category="Props")
	TSubclassOf<AActor> BP_BigStatue;

	UPROPERTY(EditDefaultsOnly, Category="Props")
	TSubclassOf<AActor> BP_PuddleGenerator;

	UPROPERTY(EditDefaultsOnly, Category="Room")
	UStaticMeshComponent* Floor;

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetFloor(UStaticMeshComponent* FloorToSet);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetGameMode();

	void SpawnPuddle(int &Iterator);

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AWall> Wall_BP;

	bool IsBiggestRoom=false;
private:
	AArtGuardGameMode* GameMode;
	AWall* CreateWall(FVector Location, FVector Scale, FRotator Rotation=FRotator::ZeroRotator);
	ARoom* UpRoom;
	ARoom* BottomRoom;
	ARoom* RightRoom;
	ARoom* LeftRoom;

	TArray<int> PossibleRotationAnglesForDecorWalls = TArray<int>{ 45, 60, 90, 120, 135, 180 };

	void CreateFrames(TArray<AWall*> Walls, FRotator Rotation, bool IsHorizontal, int FrameOffset);

	AActor* CreateProp(FVector Location, FRotator Rotation = FRotator::ZeroRotator);

	bool IsInExitLine(FVector Start, FVector End, bool DebugDraw);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGuardGameMode.h"
#include "Room.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	BIG_SQUARE_ROOM=0,
	SMALL_SQUARE_ROOM=1,
	RECTANGLE_ROOM=2,
	CIRCLE_ROOM=3
};

UENUM(BlueprintType)
enum class EOtherRoomPosition : uint8
{
	UP=0,
	BOTTOM=1,
	RIGHT=2,
	LEFT=3
};

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

	UPROPERTY(EditDefaultsOnly, Category="Walls")
	int NumberOfWallsByX;

	UPROPERTY(EditDefaultsOnly, Category="Walls")
	int NumberOfWallsByY;

	UPROPERTY(EditDefaultsOnly, Category="Room")
	UStaticMeshComponent* Floor;

	UFUNCTION(BlueprintCallable, Category="Room")
	void SetFloor(UStaticMeshComponent* FloorToSet);
	void CastToArrowArray(TArray<USceneComponent*> Array, TArray<UArrowComponent*>& OutArray);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetArrows(TArray<USceneComponent*> UpMainArrowsToSet, TArray<USceneComponent*> BottomMainArrowsToSet, TArray<USceneComponent*> LeftArrowsToSet, TArray<USceneComponent*> RightArrowsToSet);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SetGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
	TArray<UArrowComponent*> BottomMainExitArrows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
	TArray<UArrowComponent*> UpMainExitArrows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
	TArray<UArrowComponent*> LeftExitArrows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn")
	TArray<UArrowComponent*> RightExitArrows;

	UPROPERTY(BlueprintReadOnly, Category="Spawn")
	TArray<AWall*> Walls;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AWall> Wall_BP;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AExit> Exit_BP;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AExit> SideExit_BP;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	ERoomType Roomtype;

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnWalls();
	void SpawnExit(TArray<UArrowComponent*> Array);

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnExits();

	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnSideExits(TArray<UArrowComponent*> Array);

	UPROPERTY(VisibleAnywhere, Category="Spawn")
	ERoomPosition RoomPosition;

	UPROPERTY(VisibleAnywhere, Category="Grid")
	TArray<class ACell*> SpawnLocations;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	TSubclassOf<AActor> Aisle_BP;

	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupCollisionBoxes(UBoxComponent* Up, UBoxComponent* Bottom, UBoxComponent* Right, UBoxComponent* Left);

	UBoxComponent* UpCollision;
	int UpCount;
	UBoxComponent* BottomCollision;
	int BottomCount;
	UBoxComponent* RightCollision;
	int RightCount;
	UBoxComponent* LeftCollision;
	int LeftCount;

	ARoom* GetRoom(EOtherRoomPosition RoomPosition) const;
	void GetExits(EOtherRoomPosition RoomPosition);

	void RemoveRange(TArray<UArrowComponent*> &FromArray, TArray<UArrowComponent*> RangeToDelete);

	UFUNCTION()
	void OnOverlapBeginUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	AArtGuardGameMode* GameMode;

	TArray<UArrowComponent*> TotalExits;

	TArray<TArray<UArrowComponent*>> AllExits;

	ARoom* UpRoom;
	ARoom* BottomRoom;
	ARoom* RightRoom;
	ARoom* LeftRoom;
};

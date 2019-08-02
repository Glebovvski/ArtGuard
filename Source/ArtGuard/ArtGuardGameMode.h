// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArtGuardGameMode.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class ERoomPosition : uint8
{
	UPPER_LEFT = 0,
	CENTRE_LEFT = 1,
	BOTTOM_LEFT = 2,

	UPPER_CENTRE = 3,
	CENTRE = 4,
	BOTTOM_CENTRE = 5,

	UPPER_RIGHT = 6,
	CENTRE_RIGHT = 7,
	BOTTOM_RIGHT = 8
};
class ARoom;
class UMaterialInstance;
class AGrid;
class ACell;

UCLASS()
class ARTGUARD_API AArtGuardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		ERoomPosition RoomPosition;

	TArray<ERoomPosition> Positions = {
		ERoomPosition::UPPER_LEFT,
		ERoomPosition::CENTRE_LEFT,
		ERoomPosition::BOTTOM_LEFT,
		ERoomPosition::UPPER_CENTRE,
		ERoomPosition::CENTRE,
		ERoomPosition::BOTTOM_CENTRE,
		ERoomPosition::UPPER_RIGHT,
		ERoomPosition::CENTRE_RIGHT,
		ERoomPosition::BOTTOM_RIGHT
	};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Materials")
		TArray<UMaterialInstance*> VerticalMaterials;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Materials")
		TArray<UMaterialInstance*> HorizontalMaterials;

	UFUNCTION(BlueprintCallable, Category = "Materials")
		TArray<UMaterialInstance*> GetVerticalMaterials() const;

	UFUNCTION(BlueprintCallable, Category = "Materials")
		TArray<UMaterialInstance*> GetHorizontalMaterials() const;

	void RemoveUsedPictureFromArray(bool IsHorizontalArray, int index);

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
		AGrid* Grid;

	UFUNCTION(BlueprintCallable, Category = "Grid")
		void SetGrid(AGrid* GridToSet);
	int ConfigureCellToSpawnOn(ERoomPosition Position);

	UFUNCTION(BlueprintCallable, Category="EXTRA")
	void SpawnExtraRooms();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<ARoom> SmallSquareRoom_BP;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<ARoom> BigSquareRoom_BP;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<ARoom> RectangleRoom_BP;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<ARoom> RectangleVerticalRoom_BP;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<ARoom> CircleRoom_BP;

	TArray<TSubclassOf<ARoom>> RoomTypes;

	TMap<TSubclassOf<ARoom>, int> RoomsCount;

	int MaxCircleRooms=1;
	int CurrentCircleRooms=0;
	int MaxSmallRooms=9;
	int CurrentSmallRooms=0;
	int MaxRectRooms = 9;
	int CurrentRectRooms=0;
	int MaxBigRooms=5;
	int CurrentBigRooms=0;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		ARoom* SpawnRoom(ACell* Cell, TSubclassOf<ARoom> RoomToSpawn);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SpawnRooms();

	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<ARoom*> SpawnedRooms;

private:

	TArray<ACell*> RemainedCells;

	void GetFreeCellsOnPosition(ERoomPosition RoomPosition);

	UFUNCTION(BlueprintCallable, Category = "Materials")
		void GetAllMaterials();

	TArray<UMaterialInstance*> GetMaterialInstancesFromPath(FString _path);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	int GetMaxAmountOfRoomsForType(TSubclassOf<ARoom> RoomType);

	void SpawnToTheEmptyCellsLeft(ACell* Cell);

	//bool bIsLast;

protected:
};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "Area.generated.h"

class UBoxComponent;
class AWall;
//class USphereComponent;
//class ARoom;

UCLASS()
class ARTGUARD_API AArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArea();// (const FObjectInitializer& ObjectInitializer);
	void Init(int Width, int Height);
	int MaxWidth = 60;
	int MaxHeight = 60;
	int MinWidth = 22;
	int MinHeight = 22;

	float Height;
	float Width;

	FVector Location;

	UPROPERTY()
	AArea* LeftAreaChild;
	UPROPERTY()
	AArea* RightAreaChild;

	UPROPERTY()
	ARoom* Room;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Box = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Set")
		void SetBox(UStaticMeshComponent* BoxToSet);

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AArea> BP_Area;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AArea> BP_Hall;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AArea> BP_MAIN_Hall;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AWall> BP_Wall;

	UPROPERTY(VisibleAnywhere, Category = "Hall")
		FString HallText = "";
	UPROPERTY(EditDefaultsOnly,Category="Spawn")
	UMaterialInterface* HallWallMaterial;


	void SpawnChild();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;


public:
	bool Split();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Set")
		void SetGameMode();
	int MIN_AREA_SIZE;
	UPROPERTY(VisibleAnywhere, Category = "Children")
		AArea* Parent;
	UPROPERTY(VisibleAnywhere, Category = "Children")
		TArray<AActor*> Childs;
	bool HasChildren();
	bool Visited = false;
	UPROPERTY(VisibleAnywhere, Category = "Split")
		bool SplitH;

	void CreateRoom();
	void CreateRooms();
	UPROPERTY(EditDefaultsOnly, Category = "Room")
		TSubclassOf<ARoom> AreaRoom;

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void SetCollisionBoxes(UBoxComponent* Up, UBoxComponent* Down, UBoxComponent* Left, UBoxComponent* Right);

	UFUNCTION()
	void OnOverlapBeginUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginRight(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapBeginSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> NeighbourRooms;
	
	void CreateHall();

	void DeleteExtraRooms();

	UPROPERTY(EditDefaultsOnly, Category="Hall")
	TSubclassOf<class AExit> BP_Exit;

	void CreateExit(int X, int Y, bool IsRight);

	void CreateInterior();

	TArray<UBoxComponent*> CollisionArray;
	
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* UpCollision;
	UPROPERTY(VisibleAnywhere)
	FString UP;
	UPROPERTY()
		ARoom* UpRoom;
	
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* DownCollision;
	UPROPERTY(VisibleAnywhere)
		FString DOWN;
	UPROPERTY()
		ARoom* DownRoom;
	
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* RightCollision;
	UPROPERTY()
		ARoom* RightRoom;
	UPROPERTY(VisibleAnywhere)
		FString RIGHT;
	
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* LeftCollision;
	UPROPERTY()
		ARoom* LeftRoom;
	UPROPERTY(VisibleAnywhere)
		FString LEFT;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* RoomDetectionBox;
	
private:
	ARoom* GetRoom();

	//void CreateHall(ARoom* LeftRoom, ARoom* RightRoom);

	AArea* SpawnHall(int X, int Y, float Width, float Height);

	void FindCommonAreasBetweenNeighbourRooms(ARoom* thisRoom, ARoom* NeighbourRoom, float& Min, float& Max, bool isComparedByX);
	
	class AArtGuardGameMode* GameMode;
	int Max;
	FTransform LeftChildTransform;
	FTransform RightChildTransform;

	void DestroyCollisions();
	
	int HallsNumber;

};
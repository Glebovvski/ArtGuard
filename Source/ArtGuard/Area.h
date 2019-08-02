// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Area.generated.h"

class UBoxComponent;

UCLASS()
class ARTGUARD_API AArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArea();
	void Init(int Width, int Height);
	int MaxWidth = 60;
	int MaxHeight = 60;
	int MinWidth = 22;
	int MinHeight = 22;

	float Height;
	float Width;

	FVector Location;

	AArea* LeftAreaChild;
	AArea* RightAreaChild;

	class ARoom* Room;

	UStaticMeshComponent* Box;

	UFUNCTION(BlueprintCallable, Category = "Set")
		void SetBox(UStaticMeshComponent* BoxToSet);

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AArea> BP_Area;

	UPROPERTY(EditDefaultsOnly, Category = "Area")
		TSubclassOf<AArea> BP_Hall;

	UPROPERTY(VisibleAnywhere, Category = "Hall")
		FString HallText = "";

	void SpawnChild();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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

	void CreateHall();

	UPROPERTY(EditDefaultsOnly, Category="Hall")
	TSubclassOf<class AExit> BP_Exit;
private:
	ARoom* GetRoom();

	//void CreateHall(ARoom* LeftRoom, ARoom* RightRoom);

	AArea* SpawnHall(int X, int Y, float Width, float Height);

	class AArtGuardGameMode* GameMode;
	int Max;
	FTransform LeftChildTransform;
	FTransform RightChildTransform;

	void DestroyCollisions();

	TArray<UBoxComponent*> CollisionArray;
	UBoxComponent* UpCollision;
	ARoom* UpRoom;
	UBoxComponent* DownCollision;
	ARoom* DownRoom;
	UBoxComponent* RightCollision;
	ARoom* RightRoom;
	UBoxComponent* LeftCollision;
	ARoom* LeftRoom;
};



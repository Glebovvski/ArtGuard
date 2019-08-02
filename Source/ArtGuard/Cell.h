// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArtGuardGameMode.h"
#include "Cell.generated.h"

class UBoxComponent;

UCLASS()
class ARTGUARD_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

	UStaticMeshComponent* Plane;

	UPROPERTY(VisibleAnywhere, Category="Grid")
	ERoomPosition RoomPosition;

	UFUNCTION(BlueprintCallable, Category="Plane")
	void SetPlane(UStaticMeshComponent* PlaneToSet);

	UFUNCTION(BlueprintCallable, Category="Plane")
	void SetCollisionBox(UBoxComponent* BoxToSet);

	UBoxComponent* CollisionCheck;

	UPROPERTY(VisibleAnywhere, Category="Spawn")
	bool IsUsed;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	 UFUNCTION()
     void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

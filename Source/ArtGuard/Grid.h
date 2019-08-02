// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class ACell;

UCLASS()
class ARTGUARD_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	UPROPERTY(EditDefaultsOnly, Category="Grid")
	int size;

	UPROPERTY(EditDefaultsOnly, Category="Grid")
	int offset;

	UPROPERTY(EditDefaultsOnly, Category="Grid")
	TSubclassOf<ACell> Cell_BP;

	UPROPERTY(EditDefaultsOnly, Category="Grid")
	TArray<ACell*> Grid;

	UFUNCTION(BlueprintCallable, Category="Grid")
	void SpawnGrid();

	void SetOverlappingEventsOnCells();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsInRange(int Number, int Min, int Max);

};

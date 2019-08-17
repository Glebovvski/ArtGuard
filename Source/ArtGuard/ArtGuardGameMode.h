// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArtGuardGameMode.generated.h"

/**
 *
 */


class ARoom;
class UMaterialInstance;
class AArea;
class ARobber;
class UAIPerceptionComponent;

UCLASS()
class ARTGUARD_API AArtGuardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Materials")
		TArray<UMaterialInstance*> VerticalMaterials;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Materials")
		TArray<UMaterialInstance*> HorizontalMaterials;

	UFUNCTION(BlueprintCallable, Category = "Materials")
		TArray<UMaterialInstance*> GetVerticalMaterials() const;

	UFUNCTION(BlueprintCallable, Category = "Materials")
		TArray<UMaterialInstance*> GetHorizontalMaterials() const;

	void RemoveUsedPictureFromArray(bool IsHorizontalArray, int index);

	UFUNCTION(BlueprintCallable, Category="SPawn")
	void SpawnArea();

	UPROPERTY(EditDefaultsOnly, Category="Area")
	TSubclassOf<AArea> BP_Area;

	UPROPERTY(EditDefaultsOnly, Category="Robber")
	TSubclassOf<ARobber> BP_Robber;

	TArray<AArea*> Areas;

	bool IsRightExitSet=false;
	bool IsUpExitSet=false;

	AArea* MainRightExit;
	AArea* MainUpExit;

	UFUNCTION(BlueprintCallable, Category="Exit")
	AArea* GetMainRightExit();
	UFUNCTION(BlueprintCallable, Category="Exit")
	AArea* GetMainUpExit();

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnRobber();

	//UFUNCTION(BlueprintCallable, Category="SetRobber")
	//void SetSight(float SightRadius);

	//UPROPERTY(EditDefaultsOnly, Category="SetRobber")
	//AAIController* RobberAIController;
	UAIPerceptionComponent* Perception;


private:
	UFUNCTION(BlueprintCallable, Category = "Materials")
		void GetAllMaterials();

	APawn* Player;

	TArray<UMaterialInstance*> GetMaterialInstancesFromPath(FString _path);
	
	TArray<AArea*> FoundAreas;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};



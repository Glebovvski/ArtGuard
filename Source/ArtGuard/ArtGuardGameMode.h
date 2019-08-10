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

UCLASS()
class ARTGUARD_API AArtGuardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
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

	TArray<AArea*> Areas;

	bool IsRightExitSet=false;
	bool IsUpExitSet=false;

	AArea* MainRightExit;
	AArea* MainUpExit;

private:
	UFUNCTION(BlueprintCallable, Category = "Materials")
		void GetAllMaterials();

	TArray<UMaterialInstance*> GetMaterialInstancesFromPath(FString _path);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};



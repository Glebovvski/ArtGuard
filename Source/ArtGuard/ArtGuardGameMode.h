// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Bonus.h"
#include "ArtGuardGameMode.generated.h"

/**
 *
 */


class ARoom;
class UMaterialInstance;
class AArea;
class ARobber;
class AGuard;
class UAIPerceptionComponent;

UCLASS()
class ARTGUARD_API AArtGuardGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

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

	UFUNCTION(BlueprintCallable, Category="Bonus")
	void ActionBonus1();
	
	UFUNCTION(BlueprintCallable, Category="Bonus")
	void ActionBonus2();

	UFUNCTION(BlueprintCallable, Category="Bonus")
	void ActionBonus3();


	UPROPERTY(EditDefaultsOnly, Category="Area")
	TSubclassOf<AArea> BP_Area;

	UPROPERTY(EditDefaultsOnly, Category="Robber")
	TSubclassOf<ARobber> BP_Robber;
	TArray<Bonus> RobberBonuses = 
	{
		Bonus(EBonusAddition::Increase, EBonusType::RadiusVisibility, "Increase Radius for Visibility by "),
		Bonus(EBonusAddition::Increase, EBonusType::WalkSpeed, "Increase Max Speed by "),
		Bonus(EBonusAddition::Decrease, EBonusType::Loudness, "Decrease Your Loudness by "),
		Bonus(EBonusAddition::Decrease, EBonusType::CatchCone, "Decrease Guard's Catch Scale by "),
		Bonus(EBonusAddition::Increase, EBonusType::StealSpeed, "Increase Steal Speed by "),
	};
	
	UPROPERTY(EditDefaultsOnly, Category="Robber")
	TSubclassOf<AGuard> BP_Guard;
	TArray<Bonus> GuardBonuses = 
	{
		Bonus(EBonusAddition::Increase, EBonusType::RadiusVisibility, "Increase Radius for Visibility by "),
		Bonus(EBonusAddition::Increase, EBonusType::WalkSpeed, "Increase Max Speed by "),
		Bonus(EBonusAddition::Increase, EBonusType::SneakSpeed, "Increase Sneak Speed by "),
		Bonus(EBonusAddition::Decrease, EBonusType::Loudness, "Decrease Your Loudness by "),
		Bonus(EBonusAddition::Decrease, EBonusType::SneakLoudness, "Decrease Your Sneak Loudness by "),
		Bonus(EBonusAddition::Increase, EBonusType::CatchCone, "Increase Catch Cone Scale by ")
	};

	void Shuffle(TArray<Bonus> BonusArray);

	TArray<AArea*> Areas;

	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool IsGuardPlayer;

	bool IsRightExitSet=false;
	bool IsUpExitSet=false;

	AArea* MainRightExit;
	AArea* MainUpExit;

	UFUNCTION(BlueprintCallable, Category="Exit")
	AArea* GetMainRightExit();
	UFUNCTION(BlueprintCallable, Category="Exit")
	AArea* GetMainUpExit();

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SpawnAIRobber();
	FTransform GetRandomSpawnLocation();
	ARobber* Robber;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	AGuard* SpawnAIGuard();
	void CheckCollisionForDecorateWalls(float X, float Y, bool& checkCollisionInRoom);
	AGuard* Guard;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SetLocationForRobber();

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SetLocationForGuard();

	UFUNCTION(BlueprintCallable, Category="SetRobber")
	void SetRobberSight(float SightRadius);
	
	UFUNCTION(BlueprintCallable, Category="SetRobber")
	void SetRobberHearing(float HearRadius);

	UFUNCTION(BlueprintCallable, Category="SetRobber")
	void SetGuardSight(float SightRadius);
	
	UFUNCTION(BlueprintCallable, Category="SetRobber")
	void SetGuardHearing(float HearRadius);

	void SetHear(float HearRadius, UAIPerceptionComponent* Perception);
	void SetSight(float SightRadius, UAIPerceptionComponent* Perception);

	UFUNCTION(BlueprintCallable, Category="WinLose")
	void CalculateDamageToMuseum();

	UAIPerceptionComponent* RobberPerception;
	UAIPerceptionComponent* GuardPerception;

	int TotalPicturesCost=0;
	int TotalPictures=0;

	UFUNCTION(BlueprintCallable, Category="Pictures")
	int GetTotalPictureCost();
	UFUNCTION(BlueprintCallable, Category="Pictures")
	int GetTotalPictures();

private:
	UFUNCTION(BlueprintCallable, Category = "Materials")
		void GetAllMaterials();

	APawn* Player;

	TArray<UMaterialInstance*> GetMaterialInstancesFromPath(FString _path);
	
	TArray<AArea*> FoundAreas;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};



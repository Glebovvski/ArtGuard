// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_ArtGuard.generated.h"

/**
 * 
 */
UCLASS()
class ARTGUARD_API UGI_ArtGuard : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupGuardStats();
	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupRobberStats();

	void SaveGuardStats(float WalkSpeed, float SneakSpeed, float VisibilityRadius, float Loudness, float SneakLoudness, float CatchConeRadius, bool EnemyVisibility, float CatchSpeed);
	void SaveRobberStats(float WalkSpeed, float VisibilityRadius, float Loudness, float CatchConeRadius, float StealSpeed, bool VisibleExits, bool EnemyVisibility, float CatchSpeed);

	//GUARD STATS
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardWalkSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardSneakSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardVisibilityRadius;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardLoudness;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardSneakLoudness;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardCatchConeRadius;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool GuardEnemyVisibility;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	float GuardCatchSpeed;

	//ROBBER STATS
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float RobberVisibilityRadius;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float RobberWalkSpeed;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float RobberLoudness;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float RobberCatchConeRadius;
	UPROPERTY(BlueprintReadOnly, Category="Setup")
	float RobberStealSpeed;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool RobberVisibleExits;
	UPROPERTY(BlueprintReadWrite, Category="Setup")
	bool RobberEnemyVisibility;
};

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

	void SaveGuardStats(float WalkSpeed, float SneakSpeed, float VisibilityRadius, float Loudness, float SneakLoudness, float CatchConeRadius);

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

	
};

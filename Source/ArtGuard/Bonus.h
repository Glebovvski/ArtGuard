// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bonus.generated.h"

enum EBonusAddition
{
	Increase=0,
	Decrease=1
};

enum EBonusType
{
	WalkSpeed=0,
	RadiusVisibility,
	Loudness,
	StealSpeed,
	CatchCone,
	SneakSpeed,
	SneakLoudness
};

UCLASS()
class ARTGUARD_API ABonus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonus();

	static ABonus* BonusInit(EBonusAddition Addition, EBonusType Type, FString Text);

	void SetPercent(double Percent);
	EBonusAddition BonusAddition;
	EBonusType BonusType;
	FString BonusText;
	UFUNCTION(BlueprintCallable, Category="Bonus")
	FString GetBonusText();
	float BonusPercent;
	UFUNCTION(BlueprintCallable, Category="Bonus")
	float GetBonusPercent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

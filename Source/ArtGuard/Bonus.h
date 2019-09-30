// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

//UENUM()
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

class ARTGUARD_API Bonus
{
public:
	Bonus();
	Bonus(EBonusAddition Addition, EBonusType Type, FString Text);
	void SetPercent(double Percent);
	~Bonus();

	EBonusAddition BonusAddition;
	EBonusType BonusType;
	FString BonusText;
	double BonusPercent;
};

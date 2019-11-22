// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_ArtGuard.h"

void UGI_ArtGuard::SetupGuardStats()
{
	GuardWalkSpeed = 600;
	GuardSneakSpeed = 200;
	GuardVisibilityRadius = 1000;
	GuardLoudness = 1;
	GuardSneakLoudness = 0.7;
	GuardCatchConeRadius = 2;
	GuardEnemyVisibility = false;
	GuardCatchSpeed = 2;
	GuardPenaltyText = "";
}

void UGI_ArtGuard::SetupRobberStats()
{
	RobberVisibilityRadius = 1000;
	RobberWalkSpeed = 600;
	RobberLoudness = 0.0001;
	RobberCatchConeRadius = 2;
	RobberStealSpeed = 5;
	RobberVisibleExits = false;
	RobberEnemyVisibility = false;
	GuardCatchSpeed = 2;
	RobberPenaltyText = "";
}

void UGI_ArtGuard::SaveGuardStats(float WalkSpeed, float SneakSpeed, float VisibilityRadius, float Loudness,
	float SneakLoudness, float CatchConeRadius, bool EnemyVisibility, float CatchSpeed, FString PenaltyText)
{
	GuardWalkSpeed = WalkSpeed;
	GuardSneakSpeed = SneakSpeed;
	GuardVisibilityRadius = VisibilityRadius;
	GuardLoudness = Loudness;
	GuardSneakLoudness = SneakLoudness;
	GuardCatchConeRadius = CatchConeRadius;
	GuardEnemyVisibility = EnemyVisibility;
	GuardCatchSpeed = CatchSpeed;
	GuardPenaltyText = PenaltyText;
}

void UGI_ArtGuard::SaveRobberStats(float WalkSpeed, float VisibilityRadius, float Loudness, float CatchConeRadius,
	float StealSpeed, bool VisibleExits, bool EnemyVisibility, float CatchSpeed, FString PenaltyText)
{
	RobberWalkSpeed = WalkSpeed;
	RobberVisibilityRadius = VisibilityRadius;
	RobberLoudness = Loudness;
	GuardCatchConeRadius = CatchConeRadius;
	RobberStealSpeed = StealSpeed;
	RobberVisibleExits = VisibleExits;
	RobberEnemyVisibility = EnemyVisibility;
	GuardCatchSpeed = CatchSpeed;
	RobberPenaltyText = PenaltyText;
}

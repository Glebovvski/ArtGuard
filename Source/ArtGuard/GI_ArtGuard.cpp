// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_ArtGuard.h"

void UGI_ArtGuard::SetupGuardStats()
{
	GuardWalkSpeed = 600;
	GuardSneakSpeed = 200;
	GuardVisibilityRadius = 1000;
	GuardLoudness = 1;
	GuardSneakLoudness = 0.7;
	GuardCatchConeRadius = 3;
	GuardEnemyVisibility = false;
	GuardCatchSpeed = 2;
}

void UGI_ArtGuard::SetupRobberStats()
{
	RobberVisibilityRadius = 1000;
	RobberWalkSpeed = 600;
	RobberLoudness = 0.0001;
	RobberCatchConeRadius = 3;
	RobberStealSpeed = 5;
	RobberVisibleExits = false;
	RobberEnemyVisibility = false;
	GuardCatchSpeed = 2;
}

void UGI_ArtGuard::SaveGuardStats(float WalkSpeed, float SneakSpeed, float VisibilityRadius, float Loudness,
	float SneakLoudness, float CatchConeRadius, bool EnemyVisibility, float CatchSpeed)
{
	GuardWalkSpeed = WalkSpeed;
	GuardSneakSpeed = SneakSpeed;
	GuardVisibilityRadius = VisibilityRadius;
	GuardLoudness = Loudness;
	GuardSneakLoudness = SneakLoudness;
	GuardCatchConeRadius = CatchConeRadius;
	GuardEnemyVisibility = EnemyVisibility;
	GuardCatchSpeed = CatchSpeed;
}

void UGI_ArtGuard::SaveRobberStats(float WalkSpeed, float VisibilityRadius, float Loudness, float CatchConeRadius,
	float StealSpeed, bool VisibleExits, bool EnemyVisibility, float CatchSpeed)
{
	RobberWalkSpeed = WalkSpeed;
	RobberVisibilityRadius = VisibilityRadius;
	RobberLoudness = Loudness;
	GuardCatchConeRadius = CatchConeRadius;
	RobberStealSpeed = StealSpeed;
	RobberVisibleExits = VisibleExits;
	RobberEnemyVisibility = EnemyVisibility;
	GuardCatchSpeed = CatchSpeed;
}

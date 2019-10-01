// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_ArtGuard.h"

void UGI_ArtGuard::SetupGuardStats()
{
	GuardWalkSpeed = 600;
	GuardSneakSpeed = 200;
	GuardVisibilityRadius = 2000;
	GuardLoudness = 1;
	GuardSneakLoudness = 0.7;
	GuardCatchConeRadius = 3;
}

void UGI_ArtGuard::SaveGuardStats(float WalkSpeed, float SneakSpeed, float VisibilityRadius, float Loudness,
	float SneakLoudness, float CatchConeRadius)
{
	GuardWalkSpeed = WalkSpeed;
	GuardSneakSpeed = SneakSpeed;
	GuardVisibilityRadius = VisibilityRadius;
	GuardLoudness = Loudness;
	GuardSneakLoudness = SneakLoudness;
	GuardCatchConeRadius = CatchConeRadius;
}

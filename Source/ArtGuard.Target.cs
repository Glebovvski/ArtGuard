// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ArtGuardTarget : TargetRules
{
    public ArtGuardTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "ArtGuard" });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            ExtraModuleNames.Add("OnlineSubsystemGooglePlay");
            ExtraModuleNames.Add("OnlineSubsystem");
            ExtraModuleNames.Add("AndroidAdvertising");
        }
    }
}

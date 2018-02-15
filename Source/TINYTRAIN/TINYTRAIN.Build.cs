// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TINYTRAIN : ModuleRules
{
	public TINYTRAIN(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
        //PrivateDependencyModuleNames.AddRange(new string[] { "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        if (UEBuildConfiguration.bBuildEditor)
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
    }
}

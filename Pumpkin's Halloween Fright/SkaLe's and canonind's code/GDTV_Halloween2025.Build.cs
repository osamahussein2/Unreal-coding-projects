// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GDTV_Halloween2025 : ModuleRules
{
	public GDTV_Halloween2025(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"CinematicCamera", 
			"AIModule", 
			"Niagara", 
			"PhysicsCore",
            "GameplayTags",
			"RHI",
            "RenderCore",
			"UMG",
			"Slate",
			"SlateCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

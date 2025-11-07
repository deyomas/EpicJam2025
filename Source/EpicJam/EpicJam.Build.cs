// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class EpicJam : ModuleRules
{
	public EpicJam(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {  });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Mover",
			"MoverExamples",
			"EnhancedInput",
		});

		/*PrivateDependencyModuleNames.AddRange(new string[] {
			"Mover"
		});*/
		
		PublicIncludePaths.AddRange( new string[] {
			Path.Combine(ModuleDirectory, "Core/public"),
			Path.Combine(ModuleDirectory, "Examples/public"),
			Path.Combine(ModuleDirectory, "PlayerActions/public"),
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class L20231220_Recap : ModuleRules
{
	public L20231220_Recap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}

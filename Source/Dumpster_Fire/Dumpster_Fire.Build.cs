// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dumpster_Fire : ModuleRules
{
	public Dumpster_Fire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara" });
	}
}

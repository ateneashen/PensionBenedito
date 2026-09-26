// Copyright Pension Benedito, 2024. All rights reserved.

using UnrealBuildTool;

public class PensionBenedito : ModuleRules
{
    public PensionBenedito(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "UMG",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
    }
}
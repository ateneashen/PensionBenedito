// Copyright NarrativeActionKit, 2024. All rights reserved.
// Reusable framework for narrative action games (adventure, horror, mystery)

using UnrealBuildTool;

public class NarrativeActionKit : ModuleRules
{
    public NarrativeActionKit(ReadOnlyTargetRules Target) : base(Target)
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
            "NavigationSystem",
            "AIModule"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
    }
}
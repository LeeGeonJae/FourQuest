using UnrealBuildTool;

public class FQPlayer : ModuleRules
{
    public FQPlayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
using UnrealBuildTool;

public class FQArmour : ModuleRules
{
    public FQArmour(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
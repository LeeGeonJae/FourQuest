using UnrealBuildTool;

public class FQArmour : ModuleRules
{
    public FQArmour(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "FQUI" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
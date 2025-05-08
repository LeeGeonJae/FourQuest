using UnrealBuildTool;

public class FQUI : ModuleRules
{
    public FQUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
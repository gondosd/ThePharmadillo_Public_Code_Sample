using UnrealBuildTool;

public class ThePharmadilloEditor : ModuleRules
{
	public ThePharmadilloEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"ToolMenus"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ThePharmadillo",
				"UnrealEd",
				"BlueprintGraph",
				"KismetCompiler",
				"GraphEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);
	}
}
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LevelSelection : ModuleRules
{
	public LevelSelection(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		
		if (Target.Type == TargetType.Editor)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			// Dépendances privées pour l'éditeur
			PrivateDependencyModuleNames.AddRange(new string[] {
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PropertyEditor",
				"AssetRegistry",
				"EditorSubsystem"
			});
		}
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

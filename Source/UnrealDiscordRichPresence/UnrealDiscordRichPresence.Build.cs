// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UnrealDiscordRichPresence : ModuleRules
{
	public UnrealDiscordRichPresence(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../UnrealDiscordRichPresence/DiscordSDK/cpp"));
		
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../UnrealDiscordRichPresence/DiscordSDK/lib/discord_game_sdk.dll.lib"));
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

// Created by Stalker7274

using System.IO;
using UnrealBuildTool;

public class UnrealDiscordRichPresence : ModuleRules
{
	public UnrealDiscordRichPresence(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"DiscordPartnerSDK",
				"DiscordPartnerSDKLibrary"
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
			}
		);
	}
}

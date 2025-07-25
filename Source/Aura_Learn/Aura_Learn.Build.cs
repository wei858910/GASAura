// 学习使用

using UnrealBuildTool;

public class Aura_Learn : ModuleRules
{
	public Aura_Learn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput" , "GameplayAbilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags","GameplayTasks","NavigationSystem","Niagara","AIModule"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

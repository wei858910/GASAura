// 学习使用

using UnrealBuildTool;
using System.Collections.Generic;

public class Aura_LearnTarget : TargetRules
{
	public Aura_LearnTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Aura_Learn" } );
	}
}

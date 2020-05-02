// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

using UnrealBuildTool;
using System.Collections.Generic;

public class TCWMobileTarget : TargetRules
{
    public TCWMobileTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "TCWMobile" });
    }
}

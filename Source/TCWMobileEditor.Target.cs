// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

using UnrealBuildTool;
using System.Collections.Generic;

public class TCWMobileEditorTarget : TargetRules
{
    public TCWMobileEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "TCWMobile" });
    }
}

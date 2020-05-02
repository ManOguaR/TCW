// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

using UnrealBuildTool;

public class TCWMobile : ModuleRules
{
    public TCWMobile(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "TCWMobile/Public/AI",
                "TCWMobile/Public/Components",
                "TCWMobile/Public/Interfaces",
                "TCWMobile/Public/Framework",
                "TCWMobile/Public/Gameplay",
                "TCWMobile/Public/System",
                "TCWMobile/Public/UI",
               // ... add public include paths required here ...
            }
            );

        PrivateIncludePaths.AddRange(
            new string[] {
                "TCWMobile/Private/AI",
                "TCWMobile/Private/Interfaces",
                "TCWMobile/Private/Components",
                "TCWMobile/Private/Framework",
                "TCWMobile/Private/System",
                "TCWMobile/Private/UI",
                // ... add other private include paths required here ...
            }
            );


        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "PlayFab",
            "PlayFabCpp",
            "PlayFabCommon" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
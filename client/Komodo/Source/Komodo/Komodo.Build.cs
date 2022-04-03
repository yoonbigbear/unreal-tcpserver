// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using System;

using UnrealBuildTool;


public class Komodo : ModuleRules
{

    public Komodo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
			});



        PrivateDependencyModuleNames.AddRange(new string[] { "MessageLog", "Slate", "SlateCore" });

        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../External/"));

        PublicIncludePaths.Add(ThirdPartyPath);


        // 편하게가자

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Headers/"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Manager/"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/UI/"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/UI/Page/"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/UI/Window/"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Utill/"));
		
		
        ///this is added
        LoadPocketSphinx(Target, ThirdPartyPath);

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }


    //here we go
    public bool LoadPocketSphinx(ReadOnlyTargetRules Target, string InThirdPartyPatzzh)
    {
        bool isLibrarySupported = false;

        isLibrarySupported = true;

        //flatbuffers
        string LibraryPath = Path.Combine(InThirdPartyPatzzh, "flatbuffers-master");
        PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "flatbuffers.lib"));

        PublicIncludePaths.Add(Path.Combine(InThirdPartyPatzzh, "flatbuffers-master"));
        PublicIncludePaths.Add(Path.Combine(InThirdPartyPatzzh, "flatbuffers-master", "include"));

        //asio-1.20.0
        PublicIncludePaths.Add(Path.Combine(InThirdPartyPatzzh, "asio-1.20.0"));
        PublicIncludePaths.Add(Path.Combine(InThirdPartyPatzzh, "asio-1.20.0", "include"));

        //net
        LibraryPath = Path.Combine(InThirdPartyPatzzh, "net");
        PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "net.lib"));
        PublicIncludePaths.Add(Path.Combine(InThirdPartyPatzzh, "net"));




        /////where to pick the library if we're building for windows (32 or 64)
        // if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        //{



        //string LibraryName = "pocketsphinx";
        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //    LibraryPath = Path.Combine(LibraryPath, "x64");
        //}
        //else if (Target.Platform == UnrealTargetPlatform.Win32)
        //{
        //    LibraryPath = Path.Combine(LibraryPath, "Win32");
        //}

        // PublicLibraryPaths.Add(LibraryPath);


        //  }

        /////where to pick the library if we're building for mac
        //else if (Target.Platform == UnrealTargetPlatform.Mac)
        //{
        //    isLibrarySupported = true;
        //    string LibraryPath = Path.Combine(ThirdPartyPath, "PocketSphinx", "lib");
        //    //string LibraryName = "pocketsphinx";
        //    LibraryPath = Path.Combine(LibraryPath, "Mac");
        //    PublicLibraryPaths.Add(LibraryPath);
        //    PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "pocketsphinx.a"));
        //    PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "sphinxbase.a"));
        //
        //    PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "PocketSphinx", "include"));
        //}
        //add other platforms if needed...

        //and here we go.
        // Definitions.Add(string.Format("WITH_SIXENSE_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }

}

//public class MyThirdPartyLibrary : ModuleRules
//{
//	public MyThirdPartyLibrary(ReadOnlyTargetRules Target) : base(Target)
//	{
//		Type = ModuleType.External;
//
//		// Add any macros that need to be set
//		PublicDefinitions.Add("WITH_MYTHIRDPARTYLIBRARY=1");
//
//		// Add any include paths for the plugin
//		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));
//
//		// Add any import libraries or static libraries
//		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "foo.a"));
//	}
//}
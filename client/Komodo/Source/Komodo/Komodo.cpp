// Fill out your copyright notice in the Description page of Project Settings.

#include "Komodo.h"

//MessageLog
#include "MessageLog/Public/MessageLogInitializationOptions.h"
#include "MessageLog/Public/MessageLogModule.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Komodo, "Komodo" );


//void Komodo::StartupModule()
//{
//	// ...
//	// create a message log to use in my module
//	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
//	FMessageLogInitializationOptions InitOptions;
//	InitOptions.bShowPages = true;
//	InitOptions.bAllowClear = true;
//	InitOptions.bShowFilters = true;
//	MessageLogModule.RegisterLogListing("Komodo", NSLOCTEXT("Komodo", "KomodoLogLabel", "Komodo TODO"), InitOptions);
//}
//
//void Komodo::ShutdownModule()
//{
//	// ...
//	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
//	{
//		// unregister message log
//		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
//		MessageLogModule.UnregisterLogListing("Komodo");
//	}
//}
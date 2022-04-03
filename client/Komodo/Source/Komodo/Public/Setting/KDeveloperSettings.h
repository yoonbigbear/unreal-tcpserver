// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "KDeveloperSettings.generated.h"


UENUM()
enum ELOG_SETTING
{
	SHOW_ALL_LOG = 0 UMETA(DisplayName = "Show_All_Log"),
	HIDE_ALL_LOG = 1 UMETA(DisplayName = "Hide_All_Log"),
	SHOW_SELECTED_LOG = 2 UMETA(DisplayName = "Show_Selected_Log"),
};


/**
 * 
 */
UCLASS(config = Game, Defaultconfig, meta = (DisplayName = "KDeveloperSettings"))
class KOMODO_API UKDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	FString GetServerIP() { return ServerIP; }
	const int32 GetServerPort() { return ServerPort; }
	

public:
//Network Setting
	UPROPERTY(EditAnywhere, config, Category = "Setting | Network")
	FString ServerIP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, config, Category = "Setting | Network")
	int32 ServerPort = 60000;


//Log Setting

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	TEnumAsByte<ELOG_SETTING> LogOption_ = ELOG_SETTING::SHOW_ALL_LOG;

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	bool bShowAllLog_ = false;

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	bool bProtocol_Log_ = false;

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	bool bTemp_Log_ = false;

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	bool bWindow_Log_ = false;

	UPROPERTY(EditAnywhere, config, Category = "Setting | Log")
	bool bUI_Log_ = false;
	
};

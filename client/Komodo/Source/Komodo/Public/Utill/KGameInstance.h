// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateManater();

	UFUNCTION(BlueprintCallable)
	void ReleaseManater();

public:
	class UKDeveloperSettings* GetDeveloperSettings() { return KDeveloperSettings_; }
	class UKWindowManager* GetWindowManager() { return WindowManager_; }
	class UKNetworkManager* GetNetworkManager() { return NetworkManager_; }
	class UKLogManager* GetLogManager() { return LogManager_; }

	class UKTableManager* GetTableManager() { return TableManager_; }






private:
	UPROPERTY()
	class UKDeveloperSettings* KDeveloperSettings_;


	UPROPERTY()
	TArray<TWeakObjectPtr<class UKManagerBase>> Managers_;

	UPROPERTY()
	class UKWindowManager* WindowManager_;
	
	UPROPERTY()
	class UKNetworkManager* NetworkManager_;

	UPROPERTY()
	class UKLogManager* LogManager_;

	UPROPERTY()
	class UKTableManager* TableManager_;


};

//Global
class UKGameInstance* GGetGameInstance();


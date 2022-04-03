// Fill out your copyright notice in the Description page of Project Settings.


#include "KGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Setting/KDeveloperSettings.h"
//Manager
#include "KWindowManager.h"
#include "KNetworkManager.h"
#include "KLogManager.h"
#include "KTableManager.h"

void UKGameInstance::CreateManater()
{
	KDeveloperSettings_ = NewObject<UKDeveloperSettings>();

	Managers_.Empty();

	UKManagerBase* Manager = nullptr;
	if (WindowManager_ == nullptr)
	{
		WindowManager_ = NewObject<UKWindowManager>();
		Managers_.Add(TWeakObjectPtr<UKWindowManager>(WindowManager_));
	}
	
	if (NetworkManager_ == nullptr)
	{
		NetworkManager_ = NewObject<UKNetworkManager>();
		Managers_.Add(TWeakObjectPtr<UKNetworkManager>(NetworkManager_));
	}

	if (LogManager_ == nullptr)
	{
		LogManager_ = NewObject<UKLogManager>();
		Managers_.Add(TWeakObjectPtr<UKLogManager>(LogManager_));
	}

	if (TableManager_ == nullptr)
	{
		TableManager_ = NewObject<UKTableManager>();
		Managers_.Add(TWeakObjectPtr<UKTableManager>(TableManager_));
	}

	for (auto& mgr : Managers_)
	{
		if (mgr.IsValid())
		{
			if (mgr.Get())
			{
				mgr.Get()->Initialize_Manager();
			}
		}
	}

}

void UKGameInstance::ReleaseManater()
{
	KDeveloperSettings_ = nullptr;


	int32 ManagerSize = Managers_.Num();
	for (int32 i = 0; i < ManagerSize; ++i)
	{
		if (Managers_[i].IsValid())
		{
			Managers_[i]->ConditionalBeginDestroy();
		}
	}
	Managers_.Empty();

	WindowManager_ = nullptr;
	NetworkManager_ = nullptr;
	LogManager_ = nullptr;

	if (GetEngine())
	{
		GetEngine()->ForceGarbageCollection(true);
	}
}


//Global
TWeakObjectPtr<UKGameInstance> g_KGameinstance;

UKGameInstance* GGetGameInstance()
{
	if (g_KGameinstance.IsValid() == false)
	{
		if (GWorld && GWorld->GetGameInstance() )
		{
			g_KGameinstance = Cast<UKGameInstance>(UGameplayStatics::GetGameInstance(GWorld));
		}
	}

	return g_KGameinstance.Get();
}
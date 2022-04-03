// Fill out your copyright notice in the Description page of Project Settings.


#include "KTitleMainPage.h"

#include "KGameInstance.h"

#include "KWindowManager.h"
#include "KNetworkManager.h"

void UKTitleMainPage::OnLoginButton()
{
	if (GetGameInstance())
	{
		UKGameInstance* KGameInstance = Cast<UKGameInstance>(GetGameInstance());
		if (KGameInstance)
		{
			UKNetworkManager* NetworkMgr = KGameInstance->GetNetworkManager();

			if (NetworkMgr)
			{
				NetworkMgr->CSLoginReq(AccountInfo_);
			}
		}
	}
}

void UKTitleMainPage::SuccessLogin()
{
	if (GetGameInstance())
	{
		UKGameInstance* KGameInstance = Cast<UKGameInstance>(GetGameInstance());
		if (KGameInstance)
		{
			UKWindowManager* WindowMgr = KGameInstance->GetWindowManager();
			if (WindowMgr)
			{
				WindowMgr->CreateKWindow(EWindow_Table::CHARACTERSELECT_WINDOW);
			}
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Page/KCharacterSelectMainPage.h"
#include "KGameInstance.h"
#include "KNetworkManager.h"

void UKCharacterSelectMainPage::OnCreateCharacterBtn()
{
	UKGameInstance* KGameInstance = GGetGameInstance();
	if (KGameInstance)
	{
		UKNetworkManager* NetworkMgr = KGameInstance->GetNetworkManager();

		if (NetworkMgr)
		{
			NetworkMgr->CSCreateCharacterReq(CharacterInfo_);
		}
	}
}

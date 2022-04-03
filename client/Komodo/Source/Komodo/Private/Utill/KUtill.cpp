// Fill out your copyright notice in the Description page of Project Settings.


#include "Utill/KUtill.h"

#include "Kismet/GameplayStatics.h"
#include "KGameInstance.h"

class UKGameInstance* UKUtill::GetKGameInstance()
{
	return GGetGameInstance();
}

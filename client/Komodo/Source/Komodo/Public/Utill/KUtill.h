// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KGameInstance.h"
#include "UObject/NoExportTypes.h"
#include "KUtill.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKUtill : public UObject
{
	GENERATED_BODY()
	
public:
	static class UKGameInstance* GetKGameInstance();
};

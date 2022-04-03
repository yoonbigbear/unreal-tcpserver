// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KPrefab.h"
#include "KPage.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKPage : public UKPrefab
{
	GENERATED_BODY()
	
public:
	virtual void Initialize_Widget(const int32 zorder);

};

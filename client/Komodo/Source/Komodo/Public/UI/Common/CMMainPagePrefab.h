// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KPrefab.h"
#include "Styling/SlateBrush.h"
#include "CMMainPagePrefab.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UCMMainPagePrefab : public UKPrefab
{
	GENERATED_BODY()
public:
	UCMMainPagePrefab();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CM_MainPage")
	FSlateBrush bg_sbrush_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CM_MainPage")
	FText title_text_;


};

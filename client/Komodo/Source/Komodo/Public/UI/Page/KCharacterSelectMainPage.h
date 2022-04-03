// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Page/KPage.h"
#include "KCharacterSelectMainPage.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKCharacterSelectMainPage : public UKPage
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnCreateCharacterBtn(); 

protected:
	UPROPERTY(BlueprintReadWrite, Category = "CaracterInfo")
	FString CharacterInfo_;
};

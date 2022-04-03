// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KPage.h"
#include "KTitleMainPage.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKTitleMainPage : public UKPage
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnLoginButton();

	UFUNCTION(BlueprintCallable)
	void SuccessLogin(); // tempname get login packet

protected:
	UPROPERTY(BlueprintReadWrite, Category = "AccountInfo")
	FString AccountInfo_;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KPrefab.h"
#include "CMButtonPrefab.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UCMButtonPrefab : public UKPrefab
{
	GENERATED_BODY()
	
public:
	UCMButtonPrefab();

	virtual void SetVisible(const bool bSet) override;

	void SetDisable(const bool bSet);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CM_Button")
	FText btn_text_;

private:
	bool bDisable_ = false;
};

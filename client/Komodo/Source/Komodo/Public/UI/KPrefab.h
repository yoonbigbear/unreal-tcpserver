// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KPrefab.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKPrefab : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Initialize_Widget(const int32 zorder);

	virtual void SetVisible(const bool bSet);

	void SetZOrder(const int32 z) { Zorder_ = z; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KPrefab")
	int32 Zorder_;

	// SelfHitTestInvisible or Collapsed
	bool bVisible_ = false;
};

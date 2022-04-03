// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/CMButtonPrefab.h"

UCMButtonPrefab::UCMButtonPrefab()
{
	btn_text_ = FText::FromString(TEXT("Button"));
}

void UCMButtonPrefab::SetVisible(const bool bSet)
{
	if (bVisible_ == bSet)
	{
		return;
	}

	bVisible_ = bSet;

	if (bVisible_)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCMButtonPrefab::SetDisable(const bool bSet)
{
	if (bDisable_ == bSet)
	{
		return;
	}

	bDisable_ = bSet;
	// 비활성화
}

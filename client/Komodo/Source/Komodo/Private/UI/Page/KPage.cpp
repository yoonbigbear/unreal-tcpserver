// Fill out your copyright notice in the Description page of Project Settings.


#include "KPage.h"

void UKPage::Initialize_Widget(const int32 zorder)
{
	Super::Initialize_Widget(zorder);

	if (IsInViewport() == false)
	{
		AddToViewport(zorder);
	}
	else
	{
		// 새로 page추가 없이 init되는 경우 z값 업데이트는 어떻게 할지 고민해봐야힘
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KPrefab.h"

void UKPrefab::Initialize_Widget(const int32 zorder)
{
	//Prefab 초기화 함수
	//1. page가 새로 생성되는경우 호출됨.
	//2. CreatedWindows에 존재하는 상태에서 사용자가 CreateKWindow를 호출했을 때에도 호출
	SetVisible(true);
	SetZOrder(zorder);

}

void UKPrefab::SetVisible(const bool bSet)
{
	if (bVisible_ == bSet)
	{
		return;
	}

	bVisible_ = bSet;

	if (bVisible_)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
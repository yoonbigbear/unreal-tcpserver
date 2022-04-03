// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "KDataTable.generated.h"

//WindowTable
USTRUCT(BlueprintType)
struct FWindowTableEntity : public FTableRowBase
{
	GENERATED_BODY()

	public:
		FWindowTableEntity() : bfullscreen(false), bisduplicated(false), id(0), zorder(0), path(TEXT("")), name(TEXT("")) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	bool bfullscreen; // 풀스크린 window인가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	int32 bisduplicated; // 중복생성 가능한 window인가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	int32 id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	int32 zorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	FString path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WindowTableData")
	FString name;
};


UCLASS()
class KOMODO_API AKDataTable : public AActor
{
	GENERATED_BODY()
	
};

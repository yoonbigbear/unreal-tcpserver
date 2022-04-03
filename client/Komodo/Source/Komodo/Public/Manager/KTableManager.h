// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/KManagerBase.h"



#include "Table/KDataTable.h"



#include "KTableManager.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKTableManager : public UKManagerBase
{
	GENERATED_BODY()
public:
	TArray<FWindowTableEntity*> Get_Window_Table_All(bool& bOutValid);
	FWindowTableEntity* Get_Window_Table(const int32 Index);
	 
	
	
private:
	TMap<FString, UDataTable*> DataTableMap_;
};

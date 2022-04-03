// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/KTableManager.h"

FWindowTableEntity* UKTableManager::Get_Window_Table(const int32 Index)
{
    if (Index < 0)
    {
        return nullptr;
    }

    FString TableName = TEXT("Window_Table");

    if (DataTableMap_.Contains(*TableName) == false)
    {
        UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Table/Window_Table.Window_Table")/*, NULL, LOAD_None, NULL*/);
        if (DataTable)
        {
            DataTableMap_.Add(TableName, DataTable);
        }
    }

    if (DataTableMap_.Contains(*TableName) == false || DataTableMap_[*TableName] == nullptr)
    {
        return nullptr;
    }

    FName RowName = FName(FString::FromInt(Index));
    FWindowTableEntity* Entity = DataTableMap_[*TableName]->FindRow<FWindowTableEntity>(RowName, "", false);
    
    if (Entity == nullptr)
    {
        return nullptr;
    }

    return Entity;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "KManagerBase.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKManagerBase : public UObject
{
	GENERATED_BODY()
public:
			UKManagerBase() {}
	virtual ~UKManagerBase() {}
public:
	virtual void Initialize_Manager() {};
	
};

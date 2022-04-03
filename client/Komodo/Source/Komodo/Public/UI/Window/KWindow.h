// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Komodo.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KWindow.generated.h"




UCLASS()
class KOMODO_API AKWindow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWindow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void Initialize_Window(const int32 zorder, const bool bActive = true, const bool bVisible = true);
	virtual void CreatePage();

public:
	void SetActiveWindow(const bool bSet);
	void SetVisibleWindow(const bool bSet);

public:
	EWindow_Table Get_Window_ID() { return EWindow_ID_; }
	const int32 Get_ZOrder() { return Zorder_; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window | PageSetting")
	TArray<TSubclassOf<class UKPage>> MainPageSources_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window | PageSetting")
	TArray<TSubclassOf<class UKPage>> ExtraPageSources_;

	UPROPERTY()
	TArray<class UKPage*> MainPages_;

	UPROPERTY()
	TArray<class UKPage*> ExtraPages_;


	UPROPERTY(EditAnywhere, Category = "Window | PageSetting")
	EWindow_Table EWindow_ID_ = EWindow_Table::INVAILD_WINDOW;


	UPROPERTY()
	bool bActive_ = false;

	UPROPERTY()
	bool bVisible_ = false;

private:
	int32 Zorder_;



};

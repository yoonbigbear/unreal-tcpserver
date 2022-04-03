// Fill out your copyright notice in the Description page of Project Settings.


#include "KWindowManager.h"

#include "KUtill.h"
#include "KTableManager.h"

#include "KWindow.h"

void UKWindowManager::Initialize_Manager()
{
	CreatedWindows_.Empty();
	RecentWindow_ = nullptr;
	


	CreateKWindow(EWindow_Table::TITLE_WINDOW);
}

void UKWindowManager::CreateKWindow(EWindow_Table eWindowValue)
{
	if (UKUtill::GetKGameInstance())
	{
		UKTableManager* TableMgr = UKUtill::GetKGameInstance()->GetTableManager();
		if (TableMgr == nullptr)
		{
			return;
		}

		FWindowTableEntity* WindowEntity = TableMgr->Get_Window_Table(int32(eWindowValue));
		if (WindowEntity == nullptr)
		{
			return;
		}

		bool bwillcreate = true;
		int32 NewWinowZ = 0;
		if (Get_Recent_Window())
		{
			NewWinowZ = Get_Recent_Window()->Get_ZOrder() + ADD_ZORDER_VALUE_;
		}

		for (auto& window : CreatedWindows_)
		{
			if (window && eWindowValue == window->Get_Window_ID())
			{
				if (WindowEntity->bisduplicated == false) // 중복생성을 허용하지 않음
				{
					// 새로 생성되지 않더라도 명시적으로 window를 생성하는 상황이므로 Init함
					// 혹은 Init말고 적합한 함수를 생성해야 할 수도 있음.
					bwillcreate = false;
					RecentWindow_ = window;
					if (WindowEntity->bfullscreen)
					{
						AddNewWindow(bwillcreate);
					}
					window->Initialize_Window(NewWinowZ + WindowEntity->zorder);
					break;
				}
			}
		}

		if (bwillcreate) // 중복생성이 허용되거나, CreatedWindows_ 목록에 없던경우 Create
		{
			// Get Blueprint Class Path
			// Class'/path/name.name_C'
			TArray<FString> StrArr;
			if (WindowEntity->path.ParseIntoArray(StrArr, TEXT("/"), true) <= 0)
			{
				return;
			}

			FString fileName = StrArr.Last();
			FString path = FString::Printf(TEXT("Class'%s.%s_C'"), *WindowEntity->path, *fileName);

			UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path)); // Create BPClass
			if (GeneratedBP == nullptr)
			{
				return;
			}

			UWorld* World = GEngine->GameViewport->GetWorld();
			if (World == nullptr)
			{
				return;
			}

			FTransform WinLocation;
			AKWindow* NewWindow = World->SpawnActor<AKWindow>(GeneratedBP, WinLocation);
			if (NewWindow)
			{
				RecentWindow_ = NewWindow;
				AddNewWindow();
				NewWindow->CreatePage();
				CreatedWindows_.Add(NewWindow);
				NewWindow->Initialize_Window(NewWinowZ + WindowEntity->zorder); // page를 생성 이후 호출되어야 page init이 완료됨
			}
		}

	}
}

// 새로운 Window가 출력되었을 때 기존에 visible되던 window 처리할 함수
void UKWindowManager::AddNewWindow(const bool bcreate/* = true*/)
{
	if (bcreate == false) // 새로 window가 추가되지 않는경우 except recent window 
	{
		if (Get_Recent_Window())
		{
			for (auto& window : CreatedWindows_)
			{
				if (window && window != Get_Recent_Window())
				{
					window->SetVisibleWindow(false);
				}
			}
		}
	}
	else
	{   // 새로 window를 추가하는 경우 모든 window 대상
		for (auto& window : CreatedWindows_)
		{
			if (window)
			{
				window->SetVisibleWindow(false);
			}
		}
	}

}

AKWindow* UKWindowManager::Get_Recent_Window()
{
	if (RecentWindow_.IsValid())
	{
		return RecentWindow_.Get();
	}
	
	return nullptr;
}

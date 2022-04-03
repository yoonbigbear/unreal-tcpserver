// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KTextTransition.generated.h"

/**
 * 
 */
UCLASS()
class KOMODO_API UKTextTransition : public UObject
{
	GENERATED_BODY()
	
public:
	static FString TransitStringParam(const FString& inKey, const FString& inParam);




	template<typename T>
	static FString EnumToFString(T Enum)
	{
#if !WITH_EDITOR
		return "";
#else
		static FString EnumName;
		static bool bEnumNameInit = false;

		// Init it once
		if (bEnumNameInit == false)
		{
			bEnumNameInit = true;

			FString FunctionSignature(__FUNCSIG__);
			UE_LOG(LogTemp, Display, TEXT("Enum funcsig: [%s]"), *FunctionSignature);

			int32 FirstIDX = FunctionSignature.Find(TEXT("(enum "), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

			if (FirstIDX != INDEX_NONE)
			{
				FirstIDX = FirstIDX + 6;
				int32 LastIDX = (FunctionSignature.Len() - 1);

				if (FunctionSignature.IsValidIndex(FirstIDX) && FunctionSignature.IsValidIndex(LastIDX) && FirstIDX < LastIDX)
				{
					EnumName = FunctionSignature.Mid(FirstIDX, LastIDX - FirstIDX);
				}
			}
		}

		if (EnumName.IsEmpty())
			return FString("ENUM_Invalid");

		// const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, (const TCHAR *)(typeid(T).name()), true);
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
			return FString("ENUM_Invalid");

		return EnumName;

//#pragma warning(disable: 4996)
//		return EnumPtr->GetDisplayNameText(static_cast<int32>(Enum)).ToString();
#endif
	}
};

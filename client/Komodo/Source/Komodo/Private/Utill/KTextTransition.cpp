// Fill out your copyright notice in the Description page of Project Settings.


#include "KTextTransition.h"


FString UKTextTransition::TransitStringParam(const FString& inKey, const FString& inParam)
{
	TArray<FString> Params;

	inParam.ParseIntoArray(Params, TEXT(";"), true);

	FString left, right;
	for (FString& param_value : Params)
	{
		param_value.TrimStartAndEndInline();

		if (param_value.Contains("="))
		{
			param_value.Split("=", &left, &right);

			if (left == inKey)
			{
				if (right.Len() > 0)
				{
					return right;
				}
			}
			else
			{
				// invalil key
			}
		}
	}

	return FString();
}
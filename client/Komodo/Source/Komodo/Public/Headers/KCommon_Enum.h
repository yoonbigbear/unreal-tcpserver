#pragma once

#define INVALID_INDEX	-1;

#define INVALID_VALUE	 0;
#define INVALID_SIZE	 0;




UENUM(BlueprintType)
enum class EWindow_Table : uint8
{
	INVAILD_WINDOW = 0	UMETA(DisplayName = "Invaild_Window"),
	TITLE_WINDOW = 1	UMETA(DisplayName = "Title_Window"),
	CHARACTERSELECT_WINDOW = 2	UMETA(DisplayName = "CharacterSelect_Window"),
	MAX_WINDOW = 3	UMETA(DisplayName = "MAX_WINDOW"),
};

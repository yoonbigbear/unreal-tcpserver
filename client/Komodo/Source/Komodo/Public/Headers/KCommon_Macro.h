#pragma once

#define STRINGIZE(l)    #l
#define REMINDER(Line, Message)  __FILE__ "(" STRINGIZE(Line) "): "Message
#define ARG_TODO            REMINDER(__LINE__, "############ [TODO] : ")
#define LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))


// Message Log
#define __TODO(argmessage)			message (ARG_TODO #argmessage)


//// Console Log
//#define LOG_INT(var)	UE_LOG(MPLOG, Log, TEXT("%s (INT : %i) | %s [Local] %s  [Remote] %s | [CName] %s"),\
//						NETMODE_WORLD,\
//						var,\
//						*LOG_CALLINFO,\
//						*UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()),\
//						*UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()),\
//						*GetName())
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Manager/KManagerBase.h"


#include <concurrent_queue.h>

#include <flatbuffers/flatbuffers.h>
#include <SamplePacket_generated.h>
#include <asio.hpp>


#include "net_client.h"


#include "Tickable.h"

#include "KNetworkManager.generated.h"

/**
 * 
 */

//using boost::asio::ip::tcp;


enum class protocol : unsigned short
{
	kTextSend,
	kLogin,
	kCreateAccount,
	kCreateCharacter,
	kCheckCharacterNickname,

	kResultCode
};

// protocol와 일치 : for DisplayName
// TODO maxnum이 255이므로 enum string 전환 방법을 나중에 변경해야함.
enum class EDisplayProtocol : uint16
{
	kTextSend = 0 UMETA(DisplayName = "kTextSend"),
	kLogin = 1 UMETA(DisplayName = "kLogin"),
	kCreateAccount = 2 UMETA(DisplayName = "kCreateAccount"),
	kCreateCharacter = 3 UMETA(DisplayName = "kCreateCharacter"),
	kCheckCharacterNickname = 4 UMETA(DisplayName = "kCheckCharacterNickname"),
	kResultCode = 5 UMETA(DisplayName = "kResultCode")
};

enum 
{
	max_length = 1024
};




enum class result_code : unsigned short 
{
	kCreateSuccess,
	kCreateFailed,

	kAleadyExist,
	kNotExist,

	kLoginSuccess,
	kLoginFailed,
};



class CustomClient : public net::ClientInterface<protocol>
{
public:

};

UCLASS()
class KOMODO_API UKNetworkManager : public UKManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize_Manager() override;

	bool ConnectServer();
	void SendProtocol(protocol eProtocol, flatbuffers::FlatBufferBuilder& Inbuffer);
	void ReceiveProtocol();

	void CSLoginReq(const FString& Param);
	void CSCreateCharacterReq(const FString& Param);
	void CSSelectCharacterNickNameReq(const FString& Param);

	void SCLoginReq();
	void SCCreateCharacterReq();
	void SCSelectCharacterNickNameReq();
	
public:
	// Network Tick
	UFUNCTION(BlueprintCallable, Category = "NetworkTick")
	void StartNetworkTick();
	void CheckPacket();

private:
	FTimerHandle tNetHandle_;

	CustomClient C_; // static 왜안댐
	bool bloggedin_ = false;

	const float NETWORK_TICK_DELAY_ = 1.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "KNetworkManager.h"
#include "KWindowManager.h"

#include "KUtill.h"
#include "KGameInstance.h"
#include "Setting/KDeveloperSettings.h"

#include "KTextTransition.h"

void UKNetworkManager::Initialize_Manager()
{
    ConnectServer();
}

void UKNetworkManager::SendProtocol(protocol eProtocol, flatbuffers::FlatBufferBuilder& Inbuffer)
{
	// 서버로 메세지 전송
	if (C_.IsConnected())
	{
		net::Message<protocol> msg;
		msg.header.id = eProtocol;
		msg << Inbuffer;
		C_.Send(msg);
		UE_LOG(LogTemp, Log, TEXT("SendProtocol:%s"), *UKTextTransition::EnumToFString<EDisplayProtocol>(static_cast<EDisplayProtocol>(eProtocol)));
	}
	else
	{
		//
	}
}

void UKNetworkManager::ReceiveProtocol()
{
	UKGameInstance* KGameInstance = UKUtill::GetKGameInstance();
	if (KGameInstance == nullptr)
	{
		return;
	}
	try
	{
		std::thread msg_recv([this]() {

			while (1)
			{
				if (!C_.Incoming().empty())
				{
					auto msg = C_.Incoming().pop_front().msg;

					UE_LOG(LogTemp, Log, TEXT("ReceiveProtocol:%s"), *UKTextTransition::EnumToFString<EDisplayProtocol>(static_cast<EDisplayProtocol>(msg.header.id)));

					//msg 수신
					switch (msg.header.id)
					{
					case protocol::kTextSend:
					{
						auto text = flatbuffers::GetRoot<SamplePacket::textREQ>(msg.body.data());
					}
					case protocol::kResultCode:
					{
						auto res = flatbuffers::GetRoot<SamplePacket::ResultCode>(msg.body.data());
						auto code = static_cast<result_code>(res->rescode());
						switch (code)
						{
						case result_code::kLoginSuccess:
							UE_LOG(LogTemp, Log, TEXT("login success"));
							SCLoginReq();
							break;
						case result_code::kAleadyExist:
							UE_LOG(LogTemp, Log, TEXT("login kAleadyExist"));
							break;
						case result_code::kLoginFailed:
							UE_LOG(LogTemp, Log, TEXT("login kLoginFailed"));
							break;
						}
					}
					break;

					}
				}
			}});

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}

void UKNetworkManager::CSLoginReq(const FString& Param)
{
	FString id = UKTextTransition::TransitStringParam(TEXT("id"), Param);
	FString password = UKTextTransition::TransitStringParam(TEXT("pw"), Param);
	std::string str_id = TCHAR_TO_ANSI(*id);
	std::string str_pw = TCHAR_TO_ANSI(*password);
	UE_LOG(LogTemp, Log, TEXT("ID : %s"), *id);
	UE_LOG(LogTemp, Log, TEXT("ID : %s"), *password);

	flatbuffers::FlatBufferBuilder builder(1024);
	auto login_id = builder.CreateString(str_id);
	auto login_pw = builder.CreateString(str_pw);

	SamplePacket::LoginReqBuilder login_req_builder(builder);
	login_req_builder.add_id(login_id);
	login_req_builder.add_pw(login_pw);
	auto ack = login_req_builder.Finish();
	builder.Finish(ack);

	SendProtocol(protocol::kLogin, builder);
}

void UKNetworkManager::CSCreateCharacterReq(const FString& Param)
{
	FString nickname = UKTextTransition::TransitStringParam(TEXT("nickname"), Param);
	FString classname = UKTextTransition::TransitStringParam(TEXT("class"), Param);

	std::string str_nickname = TCHAR_TO_ANSI(*nickname);
	int32 classvalue = FCString::Atoi(*classname);

	flatbuffers::FlatBufferBuilder builder(1024);
	auto bf_nickname = builder.CreateString(str_nickname);

	SamplePacket::CreateCharacterReqBuilder create_character_builder(builder);
	create_character_builder.add_nickname(bf_nickname);
	create_character_builder.add_class_(classvalue); // temp
	auto ack = create_character_builder.Finish();
	builder.Finish(ack);

	SendProtocol(protocol::kCreateCharacter, builder);
}

void UKNetworkManager::CSSelectCharacterNickNameReq(const FString& Param)
{

}

void UKNetworkManager::SCLoginReq()
{
	UKGameInstance* KGameInstance = GGetGameInstance();
	if (KGameInstance)
	{
		UKWindowManager* WindowMgr = KGameInstance->GetWindowManager();
		if (WindowMgr)
		{
			WindowMgr->CreateKWindow(EWindow_Table::CHARACTERSELECT_WINDOW);
		}
	}
}

void UKNetworkManager::SCCreateCharacterReq()
{

}

void UKNetworkManager::SCSelectCharacterNickNameReq()
{

}

bool UKNetworkManager::ConnectServer()
{
    UKGameInstance* KGameInstance = UKUtill::GetKGameInstance();
    if (KGameInstance == nullptr)
    {
        return false;
    }

    UKDeveloperSettings* KDeveloperSettings = KGameInstance->GetDeveloperSettings();
    if (KDeveloperSettings == nullptr)
    {
        return false;
    }

	std::string str_ip = TCHAR_TO_ANSI(*KDeveloperSettings->GetServerIP());
	C_.Connect(str_ip, KDeveloperSettings->GetServerPort());

	if (C_.IsConnected() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectServer::ConnectFail"));
		return false;
	}

	StartNetworkTick();

	UE_LOG(LogTemp, Log, TEXT("ConnectServer::Connectd_Success"));
	return true;
}

void UKNetworkManager::StartNetworkTick()
{
	UKGameInstance* KGameInstance = UKUtill::GetKGameInstance();
	if (KGameInstance)
	{
		KGameInstance->GetWorld()->GetTimerManager().SetTimer(tNetHandle_, this, &UKNetworkManager::CheckPacket, NETWORK_TICK_DELAY_, false);
	}
}

void UKNetworkManager::CheckPacket()
{
	if (C_.IsConnected() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectServer::ConnectFail"));
		return;
	}

	if (!C_.Incoming().empty())
	{
		ReceiveProtocol();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotonChatListner.h"
#include "Console.h"

#ifdef _EG_ANDROID_PLATFORM
#	include <stdarg.h>
#	include <android/log.h>
#endif

using namespace ExitGames::Common;
using namespace ExitGames::Chat;


PhotonChatListner::PhotonChatListner(ChatListnerBase* listnerBase): ue4Listner(listnerBase)
{
}

PhotonChatListner::~PhotonChatListner()
{
}

void PhotonChatListner::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
	Console::get().debugReturn(debugLevel, string);
}

void PhotonChatListner::onStateChange(int state)
{
	Console::get().writeLine(L"Chat: onStateChange: " + state);
}

void PhotonChatListner::connectionErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"Chat: connection failed with error ") + errorCode);
}

void PhotonChatListner::clientErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"Chat: received error ") + errorCode + L" from client");
}

void PhotonChatListner::warningReturn(int warningCode)
{
	Console::get().writeLine(JString(L"Chat: received warning ") + warningCode + " from client");
}

void PhotonChatListner::serverErrorReturn(int errorCode)
{
	Console::get().writeLine(JString(L"Chat: received error ") + errorCode + " from server");
}

void PhotonChatListner::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	if (errorCode == ErrorCode::OK)
	{
		Console::get().writeLine(L"Chat: connected");
		
	}
	else
		Console::get().writeLine(JString(L"Chat: Warn: connect failed ") + errorCode + L" " + errorString);
}

void PhotonChatListner::disconnectReturn(void)
{
	Console::get().writeLine(L"Chat: disconnected");
}

void PhotonChatListner::subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels, const ExitGames::Common::JVector<bool>& results)
{
	Console::get().writeLine(L"Chat: Subscribe Created");
}

void PhotonChatListner::unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels)
{
	Console::get().writeLine(L"Chat: Unsubscribed");
}

void PhotonChatListner::onStatusUpdate(const ExitGames::Common::JString& user, int status, bool gotMessage, const ExitGames::Common::Object& message)
{
	Console::get().writeLine(L"Chat: onStateUpdate: " + user);
}

void PhotonChatListner::onGetMessages(const ExitGames::Common::JString& channelName, const ExitGames::Common::JVector<ExitGames::Common::JString>& senders, const ExitGames::Common::JVector<ExitGames::Common::Object>& messages)
{
	Console::get().writeLine(L"Chat: onGetMessages - " + channelName);
}

void PhotonChatListner::onPrivateMessage(const ExitGames::Common::JString& sender, const ExitGames::Common::Object& message, const ExitGames::Common::JString& channelName)
{
	Console::get().writeLine(L"Chat: onPrivateMessage - " + channelName);
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common-cpp/inc/Common.h"

class ChatListnerBase
{
public:
	virtual ~ChatListnerBase(void) {}
	virtual void OnConnectServer(const int errorCode, const FString& errorString) = 0;
	virtual void OnDisconnectServer(void) = 0;
	virtual void OnSubscribeChannel(const TArray<FString>& channels, const TArray<bool>& results) = 0;
	virtual void OnUnsubscribeChannel(const TArray<FString>& channels) = 0;
	virtual void OnStatusUpdate(const FString& user, const int status) = 0;
	virtual void OnGetMessage(const FString& channelName, const TArray<FString>& senders, const TArray<FString>& messages) = 0;
};

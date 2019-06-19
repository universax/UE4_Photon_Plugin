// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotonChatActor.h"

using namespace ExitGames::Common;
using namespace ExitGames::Chat;

// Sets default values
APhotonChatActor::APhotonChatActor()
	:mServerAddress("ns.exitgames.com"),
	mIsConnectedServer(false),
	mIsSubscribing(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APhotonChatActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APhotonChatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Update();
}

void APhotonChatActor::Setup(FString AppID, FString AppVersion, FString UserID)
{
	mAppID = AppID;
	mAppVersion = AppVersion;
	mUserID = UserID;

	mpListner = new PhotonChatListner(this);
	mpClient = new Client(*mpListner, TCHAR_TO_UTF8(*mAppID), TCHAR_TO_UTF8(*mAppVersion), ExitGames::Photon::ConnectionProtocol::DEFAULT);
	
	Console::get().writeLine(L"Connecting...");
	Console::get().writeLine(L"appID is set to " + ExitGames::Common::JString(TCHAR_TO_UTF8(*mAppID)));
	bool connect = mpClient->connect(AuthenticationValues().setUserID(Console::get().ToJString(mUserID)), TCHAR_TO_UTF8(*mServerAddress));
}

void APhotonChatActor::Update()
{
	if (mpClient)
	{
		mpClient->service();
	}
}

void APhotonChatActor::Disconnect()
{
	if (mpClient)
	{
		if (mIsConnectedServer)
		{
			mpClient->disconnect();
			Console::get().writeLine(L"Chat: Disconnect...");
		}
		else {
			Console::get().writeLine(L"Chat: Already disconnected...");
		}
	}
}

void APhotonChatActor::SendChatMessage(FString message)
{
	if (mpClient)
	{
		mpClient->opPublishMessage(Console::get().ToJString(mChannelName), Console::get().ToJString(message));
	}
}

void APhotonChatActor::SubscribeChannel(FString channelName)
{
	if (mpClient)
	{
		ExitGames::Common::JVector<JString> chans;
		JString targetChanName = Console::get().ToJString(channelName);
		chans.addElement(targetChanName);

		if (mpClient->opSubscribe(chans)) {
			Console::get().writeLine(L"Success Subscribe: " + Console::get().ToJString(channelName));
		}
		else {
			Console::get().writeLine(L"Error: Failed Subscribe: " + Console::get().ToJString(channelName));
		}
	}
}

void APhotonChatActor::UnsubscribeChannel(FString channelName)
{
	if (mpClient)
	{
		ExitGames::Common::JVector<JString> chans;
		JString targetChanName = Console::get().ToJString(channelName);
		chans.addElement(targetChanName);

		mpClient->opUnsubscribe(chans);
	}
}

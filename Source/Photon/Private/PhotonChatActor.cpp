// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotonChatActor.h"

using namespace ExitGames::Common;
using namespace ExitGames::Chat;

// Sets default values
APhotonChatActor::APhotonChatActor()
	:ServerAddress("ns.exitgames.com"),
	AppID("48f4b761-26cd-49a4-841f-aa0bf81124cf"),
	AppVersion("1.0")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APhotonChatActor::BeginPlay()
{
	Super::BeginPlay();
	Setup();
}

// Called every frame
void APhotonChatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Update();
}

void APhotonChatActor::Setup()
{
	mpListner = new PhotonChatListner(this);
	mpClient = new Client(*mpListner, TCHAR_TO_UTF8(*AppID), TCHAR_TO_UTF8(*AppVersion), ExitGames::Photon::ConnectionProtocol::DEFAULT);
	
	Console::get().writeLine(L"Connecting...");
	Console::get().writeLine(L"appID is set to " + ExitGames::Common::JString(TCHAR_TO_UTF8(*AppID)));
	bool connect = mpClient->connect(AuthenticationValues().setUserID(JString(L"UR") + GETTIMEMS()), TCHAR_TO_UTF8(*ServerAddress));
}

void APhotonChatActor::Update()
{
	if (mpClient)
	{
		mpClient->service();
	}
}



// ------------------------------------------------------------
JString APhotonChatActor::ToJString(FString fstr) {
	return ExitGames::Common::JString(TCHAR_TO_UTF8(*fstr));
}

FString APhotonChatActor::ToFString(ExitGames::Common::JString jstr) {
	std::string cstr = jstr.UTF8Representation().cstr();
	return cstr.c_str();
}
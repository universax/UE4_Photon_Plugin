// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotonActor.h"

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;


// Sets default values
APhotonActor::APhotonActor()
	:mServerAddress("ns.exitgames.com"),
	mIsConnectedServer(false),
	mIsJoinedRoom(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APhotonActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APhotonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Update();
}

void APhotonActor::Setup(FString AppID, FString AppVersion, FString UserID)
{
	mAppID = AppID;
	mAppVersion = AppVersion;
	mUserID = UserID;

	mpListener = new LoadBalancingListener(this);
	mpClient = new ExitGames::LoadBalancing::Client(*mpListener, TCHAR_TO_UTF8(*mAppID), TCHAR_TO_UTF8(*mAppVersion), ExitGames::Photon::ConnectionProtocol::DEFAULT, false, RegionSelectionMode::BEST);
	mpListener->setLBC(mpClient);

	Console::get().writeLine(L"Connecting...");
	Console::get().writeLine(L"appID is set to " + ExitGames::Common::JString(TCHAR_TO_UTF8(*mAppID)));
	bool connect = mpClient->connect(ExitGames::LoadBalancing::AuthenticationValues().setUserID(Console::get().ToJString(mUserID)), Console::get().ToJString(mUserID), TCHAR_TO_UTF8(*mServerAddress));
}

void APhotonActor::Update()
{
	// Need
	if (mpClient)
	{
		mpClient->service();
		mpListener->service();
	}
}

void APhotonActor::CreateRoom(FString name)
{
	if (mpListener)
	{
		mpListener->createRoom(ExitGames::Common::JString(TCHAR_TO_UTF8(*name)));
	}
	
}

TArray<FString> APhotonActor::GetRoomList()
{
	TArray<FString> names;
	if (mpClient)
	{
		const JVector<Room*>& rooms = mpClient->getRoomList();
		//const JVector<Room*>& rooms = mpListener->getClient()->getRoomList();

		for (unsigned int i = 0; i < rooms.getSize(); ++i) {
			JString name = rooms[i]->getName();
			names.Add(Console::get().ToFString(name));
			Console::get().writeLine(L"Room: " + name);
		}
	}
	return names;
}

void APhotonActor::JoinRoom(FString name)
{
	if (mpClient && !mIsJoinedRoom)
	{
		mpClient->opJoinRoom(Console::get().ToJString(name));
	}
}

void APhotonActor::LeaveRoom()
{
	if (mIsJoinedRoom) {
		if (mpClient->opLeaveRoom()) {
			Console::get().writeLine(L"Success Leave Room");
			mIsJoinedRoom = false;
		}
		else {
			Console::get().writeLine(L"Error: Leave Room");
		}
	}
	else {
		Console::get().writeLine(L"Error: Not Joined any rooms");
	}
}

bool APhotonActor::GetIsInRoom()
{
	if (mpClient)
	{
		return mpClient->getIsInGameRoom();
	}
	
	return false;
}

void APhotonActor::SendLocalTransform(FTransform transform)
{
	// Location
	FVector location = transform.GetLocation();
	// Rotation
	FQuat rotation = transform.GetRotation();

	Hashtable data;
	float posture[] = { 
		static_cast<float>(location.X),
		static_cast<float>(location.Y),
		static_cast<float>(location.Z),
		static_cast<float>(rotation.X),
		static_cast<float>(rotation.Y),
		static_cast<float>(rotation.Z),
		static_cast<float>(rotation.W)
	};
	data.put((nByte)1, posture, 7);
	if (mpClient && mIsJoinedRoom)
	{
		mpClient->opRaiseEvent(false, data, 2);
	}
	
}
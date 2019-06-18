// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Photon.h"

#include "ListnerBase.h"
#include "LoadBalancingListener.h"
#include "Console.h"

#include "PhotonPlayer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhotonActor.generated.h"

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FJoinRoomDelegate, int32, playerNr, FString, playerName, bool, local, const TArray<UPhotonPlayer*>&, players);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerTransformDelegate, int32, playerNr, FVector, pos, FRotator, rot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaveRoomDelegate, int32, playerNr);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerConnectionDelegate, bool, isConnected);

UCLASS()
class PHOTON_API APhotonActor : public AActor, public ListnerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhotonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	// Func
	UFUNCTION(BlueprintCallable, Category = "Photon | Common")
		void Setup(FString AppID, FString AppVersion, FString UserID);
	UFUNCTION(BlueprintCallable, Category = "Photon | Common")
		void Update();

	// For Debug
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		void Disconnect() {
		mpClient->disconnect();
	}
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		void CreateRoom(FString name);
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		TArray<FString> GetRoomList();
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		void JoinRoom(FString name);
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		void LeaveRoom();
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		bool GetIsInRoom();


	// Callback from Listner
	UPROPERTY(BlueprintAssignable, Category = "Photon | Callback")
	FJoinRoomDelegate OnJoinRoomEventDelegate;
	virtual void OnJoinRoomEventAction(int playerNr, const ExitGames::Common::JString& playerName, bool local) {
		Console::get().writeLine(L"OnJoinRoomEventAction");
		if (local)
		{
			mIsJoinedRoom = true;
		}
		ExitGames::LoadBalancing::MutableRoom room = mpClient->getCurrentlyJoinedRoom();
		ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*> players = room.getPlayers();
		TArray<UPhotonPlayer*> photonPlayers;
		for (int i = 0; i < (int)room.getPlayerCount(); i++) {
			UPhotonPlayer* p = NewObject<UPhotonPlayer>();
			p->SetPlayer(players[i]);
			photonPlayers.AddUnique(p);
		}
		OnJoinRoomEventDelegate.Broadcast(playerNr, ToFString(playerName), local, photonPlayers);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon | Callback")
	FLeaveRoomDelegate OnLeaveRoomEventDelegate;
	virtual void OnLeaveRoomEventAction(int playerNr) {
		Console::get().writeLine(L"OnLeaveRoomEventAction");
		OnLeaveRoomEventDelegate.Broadcast(playerNr);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon | Callback")
		FPlayerTransformDelegate OnPlayerTransformDelegate;
	virtual void OnChangePlayerPos(int playerNr, float x, float y, float z, float qx, float qy, float qz, float qw) {
		//Console::get().writeLine(L"OnChangePlayerPos" + ToJString(FString::SanitizeFloat(x)) + ", " + ToJString(FString::SanitizeFloat(y)) + ", " + ToJString(FString::SanitizeFloat(z)));
		FVector p(x, y, z);
		FQuat q(qx, qy, qz, qw);
		FRotator r(q);
		OnPlayerTransformDelegate.Broadcast(playerNr, p, r);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon | Callback")
		FServerConnectionDelegate OnServerConnectionDelegate;
	virtual void OnConnectServer(void) {
		OnServerConnectionDelegate.Broadcast(true);
		mIsConnectedServer = true;
	}
	virtual void OnDisconnectServer(void) {
		OnServerConnectionDelegate.Broadcast(false);
		mIsConnectedServer = false;
	}

	// Send local player event
	// Spawn
	// Transform
	UFUNCTION(BlueprintCallable, Category = "Photon | Debug")
		void SendLocalTransform(FTransform transform);

	// Receive other player event
	// When other player joined
	// Spawn

	// Transform


private:
	// Prop
	FString mServerAddress;
	FString mAppID;
	FString mAppVersion;
	FString mUserID;

	ExitGames::LoadBalancing::Client* mpClient;
	LoadBalancingListener* mpListener;

	// Flags
	bool mIsConnectedServer;
	bool mIsJoinedRoom;

	// Util
	ExitGames::Common::JString ToJString(FString fstr);
	FString ToFString(ExitGames::Common::JString jstr);

};

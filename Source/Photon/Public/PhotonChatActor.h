// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Photon.h"

#include "ChatListnerBase.h"
#include "PhotonChatListner.h"
#include "Console.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhotonChatActor.generated.h"


 // Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FConnectChatServerDelegate, int, errorCode, const FString&, errorString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisConnectChatServerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSubscribeChannelDelegate, const TArray<FString>&, channels, const TArray<bool>&, results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnsubscribeChannelDelegate, const TArray<FString>&, channels);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatStateUpdateDelegate, const FString&, user, const int, status);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGetChatMessageDelegate, const FString&, channelName, const TArray<FString>&, senders, const TArray<FString>&, messages);


UCLASS()
class PHOTON_API APhotonChatActor : public AActor, public ChatListnerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhotonChatActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Func
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Common")
		void Setup(FString AppID, FString AppVersion, FString UserID);
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Common")
		void Update();

	// Action
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Debug")
		void Disconnect();
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Debug")
		void SendChatMessage(FString message);
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Debug")
		void SubscribeChannel(FString channelName);
	UFUNCTION(BlueprintCallable, Category = "Photon Chat | Debug")
		void UnsubscribeChannel(FString channelName);


	// Callback from listner
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FConnectChatServerDelegate OnConnectChatServerDelegate;
	virtual void OnConnectServer(const int errorCode, const FString& errorString) {
		mIsConnectedServer = true;
		OnConnectChatServerDelegate.Broadcast(errorCode, errorString);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FDisConnectChatServerDelegate OnDisconnectChatServerDelegate;
	virtual void OnDisconnectServer(void) {
		mIsConnectedServer = false;
		OnDisconnectChatServerDelegate.Broadcast();
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FSubscribeChannelDelegate OnSubscribeChannelDelegate;
	virtual void OnSubscribeChannel(const TArray<FString>& channels, const TArray<bool>& results) {
		mIsSubscribing = true;
		mChannelName = channels[0];
		OnSubscribeChannelDelegate.Broadcast(channels, results);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FUnsubscribeChannelDelegate OnUnsubscribeChannelDelegate;
	virtual void OnUnsubscribeChannel(const TArray<FString>& channels) {
		mIsSubscribing = false;
		OnUnsubscribeChannelDelegate.Broadcast(channels);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FChatStateUpdateDelegate OnStateUpdateDelegate;
	virtual void OnStatusUpdate(const FString& user, const int status) {
		OnStateUpdateDelegate.Broadcast(user, status);
	}
	UPROPERTY(BlueprintAssignable, Category = "Photon Chat | Callback")
		FGetChatMessageDelegate OnGetChatMessageDelegate;
	virtual void OnGetMessage(const FString& channelName, const TArray<FString>& senders, const TArray<FString>& messages) {
		OnGetChatMessageDelegate.Broadcast(channelName, senders, messages);
	}


private:
	// Prop
	FString mServerAddress;
	FString mAppID;
	FString mAppVersion;
	FString mUserID;

	ExitGames::Chat::Client* mpClient;
	PhotonChatListner* mpListner;

	FString mChannelName;

	// Flags
	bool mIsConnectedServer;
	bool mIsSubscribing;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Photon.h"

#include "ChatListnerBase.h"
#include "PhotonChatListner.h"
#include "Console.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhotonChatActor.generated.h"

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
	// Prop
	UPROPERTY(EditAnywhere, Category = "Photon | Common")
		FString ServerAddress;
	UPROPERTY(EditAnywhere, Category = "Photon | Common")
		FString AppID;
	UPROPERTY(EditAnywhere, Category = "Photon | Common")
		FString AppVersion;

	// Func
	UFUNCTION(BlueprintCallable, Category = "Photon | Common")
		void Setup();
	UFUNCTION(BlueprintCallable, Category = "Photon | Common")
		void Update();

private:
	ExitGames::Chat::Client* mpClient;
	PhotonChatListner* mpListner;

	// Util
	ExitGames::Common::JString ToJString(FString fstr);
	FString ToFString(ExitGames::Common::JString jstr);
};

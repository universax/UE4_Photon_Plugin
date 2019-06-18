// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chat-cpp/inc/Client.h"
#include "ChatListnerBase.h"

/**
 * 
 */
class PhotonChatListner: public ExitGames::Chat::Listener
{
public:
	PhotonChatListner(ChatListnerBase* listnerBase);
	~PhotonChatListner(void);

	// Delegate for UE4
	ChatListnerBase* ue4Listner = nullptr;

private:
	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	/**
		Notifies app that client state changed.
		@param state new client state
		@sa ClientState::ClientState*/
	virtual void onStateChange(int state);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// callbacks for operations on server

	/**
		Client is connected now.*/
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);

	/**
		Disconnection happened.*/
	virtual void disconnectReturn(void);

	/**
		The result of the subscribe operation. Returns per channel name if the channel is now subscribed.
		@param channels channel names
		@param results per channel result: true if subscribed*/
	virtual void subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels, const ExitGames::Common::JVector<bool>& results);

	/**
		Result of unsubscribe operation. Returns per channel name if the channel is now subscribed.
		@param channels channel names that are no longer subscribed*/
	virtual void unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels);

	/**
		The new status of another user (you get updates for users that are in your friends list).
		@param user name of the user
		@param status new status of that user
		@param gotMessage true if the status contains a message you should cache locally. False: This status update does not include a message (keep any you have).
		@param message message that user set*/
	virtual void onStatusUpdate(const ExitGames::Common::JString& user, int status, bool gotMessage, const ExitGames::Common::Object& message);

	/**
		Notifies the app that the client got new messages from the server
		Number of senders is equal to number of messages in 'messages'. Sender with number '0' corresponds to message with
		number '0', sender with number '1' corresponds to message with number '1' and so on
		@param channelName channel from where messages came
		@param senders list of users who sent messages
		@param messages list of messages it self*/
	virtual void onGetMessages(const ExitGames::Common::JString& channelName, const ExitGames::Common::JVector<ExitGames::Common::JString>& senders, const ExitGames::Common::JVector<ExitGames::Common::Object>& messages);

	/**
		Notifies the app about a private message
		@param sender user who sent this message
		@param message the message itself
		@param channelName the channel name for private messages (messages that you sent yourself get added to a channel per target username)*/
	virtual void onPrivateMessage(const ExitGames::Common::JString& sender, const ExitGames::Common::Object& message, const ExitGames::Common::JString& channelName);

};

// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "SessionManager.h"
#include "../TCWMobile.h"

#include "OnlineSubsystem.h"
#include "Engine/Engine.h"

USessionManager::USessionManager()
{
	//PRE - 17
	//EXE-4
	/** Bind function for CREATING a Session */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnStartOnlineGameComplete);
	/** Bind function for FINDING a Session */
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionManager::OnFindSessionsComplete);
	/** Bind function for JOINING a Session */
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionManager::OnJoinSessionComplete);
	/** Bind function for DESTROYED a Session */
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionManager::OnDestroySessionComplete);
}

void USessionManager::DestroySession()
{
	this->DestroySession(GameSessionName);
}

void USessionManager::DestroySession(FName SessionName)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(SessionName);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}

	//DestroySession(GameSessionName, FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionManager::OnDestroySessionComplete));
}

void USessionManager::DestroySession(FName SessionName, FOnDestroySessionCompleteDelegate& Delegate)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(Delegate);

			Sessions->DestroySession(SessionName);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}
}

void USessionManager::DestroyPlayerSession(APlayerController* const PlayerController)
{
}

bool USessionManager::HasActiveSession()
{
	return false;
}

void USessionManager::FindSessions()
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bEnableLan;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bEnablePresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bEnablePresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid %s"), Sessions.IsValid() ? "UserId" : "Sessions");
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

bool USessionManager::JoinFoundSession(const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, GameSessionName, SearchResult);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid %s"), Sessions.IsValid() ? "UserId" : "Sessions");
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}

	return bSuccessful;
}


void USessionManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(TCWLog, Verbose, TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful);

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}
}

void USessionManager::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(TCWLog, Verbose, TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful);

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	//if (bWasSuccessful)
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), FindObject<UEnum>(ANY_PACKAGE, TEXT("EArenaList"), true)->GetValueAsName(Arena), true, "listen");
	//}
}

void USessionManager::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(TCWLog, Verbose, TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful);

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			UE_LOG(TCWLog, Verbose, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					UE_LOG(TCWLog, Verbose, TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName));
				}
			}
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}
}

void USessionManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(TCWLog, Verbose, TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			//APlayerController* const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			//if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			//{
			//	// Finally call the ClienTravel. If you want, you could print the TravelURL to see
			//	// how it really looks like
			//	PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			//}
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}
}

void USessionManager::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(TCWLog, Verbose, TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful);

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			UE_LOG(TCWLog, VeryVerbose, TEXT("OnDestroySessionComplete delegate handle cleared"));
		}
		else
		{
			UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get valid Sessions"));
		}
	}
	else
	{
		UE_LOG(TCWLogErrors, Warning, TEXT("Failed to get OnlineSubsystem"));
	}
}

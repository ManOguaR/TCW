// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"

#include "Account/AccountData.h"
#include "SystemEnums.h"

#include "PlayFabManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayFabManagerEvent);

UCLASS()
class TCWMOBILE_API UPlayFabManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "TCW Account Callback")
		FPlayFabManagerEvent OnOperationSuccess;
	UPROPERTY(BlueprintAssignable, Category = "TCW Account Callback")
		FPlayFabManagerEvent OnOperationFailure;

public:
	UPlayFabManager();

	/*
	*   Retrieve and the USaveGame object that stores login data.
	*   TODO: More secure method to store user account sensible data
	*/
	bool HasStoredLogin();
	/*
	*   Check if playfab client is logged in
	*/
	bool IsLoggedIn();

#pragma region User Login
	/*
	*   Automatic selection of login mode.
	*/
	void StoredLogin();
	/*
	*   Default first time loging without any data (roaming login).
	*/
	void AnonymousLogin();
	/*
	*   Login with username and password, new or existing accounts.
	*   TODO: use only for existing accounts.
	*/
	void EmailAddressLogin(FString email, FString password);
	/*
	*   Login with username and password, new or existing accounts.
	*   TODO: use only for existing accounts.
	*/
	UFUNCTION(BlueprintCallable, Category = "TCW Account")
		void UsernameLogin(FString username, FString password);

#if PLATFORM_ANDROID || PLATFORM_IOS
	/*
	*   Login with device data, only for mobile builds.
	*/
	void DeviceLogin();
#endif

	/*
	*   Create new player account.
	*/
	UFUNCTION(BlueprintCallable, Category = "TCW Account")
		void CreateAccount(FString username, FString email, FString password);

private:
	void OnLoginSuccess(const PlayFab::ClientModels::FLoginResult& Result);
	void OnLoginError(const PlayFab::FPlayFabCppError& ErrorResult);

	void OnCreateAccountSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& Result);
	void OnCreateAccountError(const PlayFab::FPlayFabCppError& ErrorResult);

#pragma endregion

#pragma region Player Statistics
public:
	/*
	*   Read current user data.
	*/
	void GetPlayerStats();
	void SetPlayerStats();
private:
	void OnGetPlayerStatsSuccess(const PlayFab::ClientModels::FGetPlayerStatisticsResult& Result);
	void OnUpdatePlayerStatsSuccess(const PlayFab::ClientModels::FUpdatePlayerStatisticsResult& Result);
	void OnPlayerStatsError(const PlayFab::FPlayFabCppError& ErrorResult);

#pragma endregion

#pragma region Profile
public:
	/*
	*   Read current user data.
	*/
	void GetPlayerProfile();

private:
	void OnGetPlayerProfileSuccess(const PlayFab::ClientModels::FGetPlayerProfileResult& Result);
	void OnGetPlayerProfileError(const PlayFab::FPlayFabCppError& ErrorResult);

#pragma endregion

#pragma region Friends
public:
	/*
	*   Read current user data.
	*/
	void GetPlayerFriendsList();

private:
	void OnGetPlayerFriendsListSuccess(const PlayFab::ClientModels::FGetFriendsListResult& Result);
	void OnGetPlayerFriendsListError(const PlayFab::FPlayFabCppError& ErrorResult);

#pragma endregion

public:
	// Begin UObject interface
	UWorld* GetWorld() const override;
	// End UObject interface

private:
	PlayFabClientPtr clientAPI = nullptr;

	UAccountData* accountData = nullptr;
	EAccountState AccountState = EAccountState::Account_Unknown;

	bool HandleErrorCode(const int32);

	bool NewlyCreatedUser;
	FString CurrentPlayFabId;
	FString CurrentSessionTicket;
};
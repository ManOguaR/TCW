// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "PlayFabManager.h"
#include "../TCWMobile.h"
#include "SystemFunctionLibrary.h"
#include "MiscFunctionLibrary.h"

#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "PlayFabClientModels.h"

UPlayFabManager::UPlayFabManager()
{
	//PRE - 16
	//EXE-3
	//Set static data
	if (clientAPI == nullptr)
		clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	clientAPI->SetTitleId(TEXT("277DD"));
}

bool UPlayFabManager::HasStoredLogin()
{
	// Retrieve and cast the USaveGame object to UAccountProfileData.
	if ((accountData = Cast<UAccountData>(UGameplayStatics::LoadGameFromSlot(TEXT("AccountSaveSlot"), 0))))
		AccountState = EAccountState::Account_NotLogged;
	else
		AccountState = EAccountState::Account_Unknown;

	//Check if exists
	return AccountState != EAccountState::Account_Unknown;
}

bool UPlayFabManager::IsLoggedIn()
{
	return clientAPI->IsClientLoggedIn();
}

#pragma region User Login
void UPlayFabManager::StoredLogin()
{
	if (!accountData->IsValidLowLevel())
	{
		USystemFunctionLibrary::DisplayError(this, TEXT("Invalid stored account.\nPlease reinstall the game."));
		UE_LOG(TCWLogErrors, Fatal, TEXT("Invalid stored account."));
	}

	if (accountData->AccountType == EAccountType::AccountType_Anonymous)
#if PLATFORM_ANDROID || PLATFORM_IOS
		DeviceLogin();
#else
		AnonymousLogin();
#endif
	else if (accountData->AccountType == EAccountType::AccountType_Email)
		EmailAddressLogin(accountData->UserEmail, accountData->UserPassword);
	else if (accountData->AccountType == EAccountType::AccountType_Google)
	{
#if PLATFORM_ANDROID
		DeviceLogin();
#else
		USystemFunctionLibrary::DisplayError(this, TEXT("Something went wrong."));
#endif
	}
	else if (accountData->AccountType == EAccountType::AccountType_Facebook)
	{
		//TODO: FACEBOOK LOGIN
		USystemFunctionLibrary::DisplayError(this, TEXT("Not Implemented."));
	}
	else if (accountData->AccountType == EAccountType::AccountType_Apple)
	{
#if PLATFORM_IOS
		DeviceLogin();
#else
		USystemFunctionLibrary::DisplayError(this, TEXT("Something went wrong."));
#endif
	}
}

void UPlayFabManager::AnonymousLogin()
{
	PlayFab::ClientModels::FLoginWithCustomIDRequest request;

	if (!accountData->IsValidLowLevel())
	{
		accountData = Cast<UAccountData>(UGameplayStatics::CreateSaveGameObject(UAccountData::StaticClass()));

		// Set data on the savegame object.
		accountData->UserName = FGuid::NewGuid().ToString();
		accountData->AccountType = EAccountType::AccountType_Anonymous;

		request.CreateAccount = true;
	}
	else
		request.CreateAccount = false;

	request.CustomId = accountData->UserName;

	clientAPI->LoginWithCustomID(request,
		PlayFab::UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateUObject(this, &UPlayFabManager::OnLoginSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnLoginError)
	);
}

void UPlayFabManager::EmailAddressLogin(FString email, FString password)
{
	PlayFab::ClientModels::FLoginWithEmailAddressRequest request;
	request.Email = email;
	request.Password = password;

	clientAPI->LoginWithEmailAddress(request,
		PlayFab::UPlayFabClientAPI::FLoginWithEmailAddressDelegate::CreateUObject(this, &UPlayFabManager::OnLoginSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnLoginError)
	);
}

void UPlayFabManager::UsernameLogin(FString username, FString password)
{
	PlayFab::ClientModels::FLoginWithPlayFabRequest request;
	request.Username = username;
	request.Password = password;

	clientAPI->LoginWithPlayFab(request,
		PlayFab::UPlayFabClientAPI::FLoginWithEmailAddressDelegate::CreateUObject(this, &UPlayFabManager::OnLoginSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnLoginError)
	);
}

#if PLATFORM_ANDROID || PLATFORM_IOS
void UPlayFabManager::DeviceLogin()
{
#if PLATFORM_ANDROID
	PlayFab::ClientModels::FLoginWithAndroidDeviceIDRequest request;
#elif PLATFORM_IOS
	PlayFab::ClientModels::FLoginWithIOSDeviceIDRequest request;
#endif

	if (!accountData->IsValidLowLevel())
	{
		accountData = Cast<UAccountProfileData>(UGameplayStatics::CreateSaveGameObject(UAccountProfileData::StaticClass()));

		// Set data on the savegame object.
		accountData->UserName = FPlatformMisc::GetUniqueDeviceId();
#if PLATFORM_ANDROID
		accountData->AccountType = EAccountType::AccountType_Google;
#elif PLATFORM_IOS
		accountData->AccountType = EAccountType::AccountType_Apple;
#endif
		request.CreateAccount = true;
	}
	else
		request.CreateAccount = false;

#if PLATFORM_ANDROID
	//TODO: GET ANDROID DEVICE ID
	request.AndroidDeviceId = accountData->UserName;
#elif PLATFORM_IOS
	//TODO: GET IOS DEVICE ID
	request.DeviceId = accountData->UserName;
#endif

#if PLATFORM_ANDROID
	clientAPI->LoginWithAndroidDeviceID(request,
		PlayFab::UPlayFabClientAPI::FLoginWithAndroidDeviceIDDelegate::CreateUObject(this, &UPlayFabManager::OnLoginSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnLoginError)
	);
#elif PLATFORM_IOS
	clientAPI->LoginWithIOSDeviceID(request,
		PlayFab::UPlayFabClientAPI::FLoginWithIOSDeviceIDDelegate::CreateUObject(this, &UPlayFabManager::OnLoginSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnLoginError)
	);
#endif
}
#endif

void UPlayFabManager::CreateAccount(FString username, FString email, FString password)
{
	PlayFab::ClientModels::FRegisterPlayFabUserRequest request;
	request.RequireBothUsernameAndEmail = true;

	request.DisplayName = ((*username == nullptr) || (username.IsEmpty())) ? (accountData->IsValidLowLevel() ? accountData->UserName : FGuid::NewGuid().ToString()) : username;

	if ((*username == nullptr) || (username.IsEmpty()))
	{
		if (accountData->IsValidLowLevel())
			username = accountData->UserName;
		else
			username = FGuid::NewGuid().ToString();
	}

	if (!accountData->IsValidLowLevel())
		accountData = Cast<UAccountData>(UGameplayStatics::CreateSaveGameObject(UAccountData::StaticClass()));

	// Set data on the savegame object.
	accountData->UserName = username;
	accountData->UserEmail = email;
	accountData->UserPassword = password;
	accountData->AccountType = EAccountType::AccountType_Email;

	request.DisplayName = accountData->UserName;
	request.Email = accountData->UserName;
	request.Password = accountData->UserPassword;


	clientAPI->RegisterPlayFabUser(request,
		PlayFab::UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(this, &UPlayFabManager::OnCreateAccountSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnCreateAccountError)
	);
}

void UPlayFabManager::OnLoginSuccess(const PlayFab::ClientModels::FLoginResult& Result)
{
	UE_LOG(TCWLogInit, Log, TEXT("Login successful; user: %s"), *Result.PlayFabId);

	NewlyCreatedUser = Result.NewlyCreated;
	CurrentPlayFabId = Result.PlayFabId;
	CurrentSessionTicket = Result.SessionTicket;

	// Save the data immediately.
	if (UGameplayStatics::SaveGameToSlot(accountData, accountData->SaveSlotName, accountData->UserIndex))
	{
		UE_LOG(TCWLogInit, Log, TEXT("Account data saved; AccountType: %s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccountType"), true)->GetValueAsString(accountData->AccountType));
		if (OnOperationSuccess.IsBound())
			OnOperationSuccess.Broadcast();
	}
	else
	{
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidParams : The API request object sent to server has invalid parameters and cannot be executed"));

		UE_LOG(TCWLogErrors, Log, TEXT("Failed to save account; AccountType: %s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccountType"), true)->GetValueAsString(accountData->AccountType));
	}
}

void UPlayFabManager::OnLoginError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (!HandleErrorCode(ErrorResult.ErrorCode))
	{
		UE_LOG(TCWLogErrors, Error, TEXT("Unhandled playfab error at login.\nHere's some debug information:\n%s"), *ErrorResult.GenerateErrorReport());
		if (OnOperationFailure.IsBound())
			OnOperationFailure.Broadcast();
	}
}

void UPlayFabManager::OnCreateAccountSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& Result)
{
	UE_LOG(TCWLogInit, Log, TEXT("Account create successful; user: %s"), *Result.PlayFabId);

	NewlyCreatedUser = true;
	CurrentPlayFabId = Result.PlayFabId;
	CurrentSessionTicket = Result.SessionTicket;

	// Save the data immediately.
	if (UGameplayStatics::SaveGameToSlot(accountData, accountData->SaveSlotName, accountData->UserIndex))
	{
		UE_LOG(TCWLogInit, Log, TEXT("Account data saved; AccountType: %s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccountType"), true)->GetValueAsString(accountData->AccountType));
		if (OnOperationSuccess.IsBound())
			OnOperationSuccess.Broadcast();
	}
	else
	{
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidParams : The API request object sent to server has invalid parameters and cannot be executed"));

		UE_LOG(TCWLogErrors, Log, TEXT("Failed to save account; AccountType: %s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccountType"), true)->GetValueAsString(accountData->AccountType));
	}
}

void UPlayFabManager::OnCreateAccountError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (!HandleErrorCode(ErrorResult.ErrorCode))
	{
		UE_LOG(TCWLogErrors, Error, TEXT("Something went wrong with login.\nHere's some debug information:\n%s"), *ErrorResult.GenerateErrorReport());
		if (OnOperationFailure.IsBound())
			OnOperationFailure.Broadcast();
	}
}
#pragma endregion

#pragma region Player Statistics
void UPlayFabManager::GetPlayerStats()
{
	PlayFab::ClientModels::FGetPlayerStatisticsRequest request;
	clientAPI->GetPlayerStatistics(request,
		PlayFab::UPlayFabClientAPI::FGetPlayerStatisticsDelegate::CreateUObject(this, &UPlayFabManager::OnGetPlayerStatsSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnPlayerStatsError)
	);
}

void UPlayFabManager::SetPlayerStats()
{
	PlayFab::ClientModels::FUpdatePlayerStatisticsRequest request;
	request.Statistics = {
		PlayFab::ClientModels::FStatisticUpdate(),// { StatisticName = "", Value = "" },
		PlayFab::ClientModels::FStatisticUpdate(),// { StatisticName = "", Value = "" },
		PlayFab::ClientModels::FStatisticUpdate(),// { StatisticName = "", Value = "" },
		PlayFab::ClientModels::FStatisticUpdate(),// { StatisticName = "", Value = "" }
	};
	clientAPI->UpdatePlayerStatistics(request,
		PlayFab::UPlayFabClientAPI::FUpdatePlayerStatisticsDelegate::CreateUObject(this,&UPlayFabManager::OnUpdatePlayerStatsSuccess),
			PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnPlayerStatsError)
		);
}

void UPlayFabManager::OnGetPlayerStatsSuccess(const PlayFab::ClientModels::FGetPlayerStatisticsResult& Result)
{
	if (OnOperationSuccess.IsBound())
		OnOperationSuccess.Broadcast();
}

void UPlayFabManager::OnUpdatePlayerStatsSuccess(const PlayFab::ClientModels::FUpdatePlayerStatisticsResult& Result)
{
	if (OnOperationSuccess.IsBound())
		OnOperationSuccess.Broadcast();
}

void UPlayFabManager::OnPlayerStatsError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (OnOperationFailure.IsBound())
		OnOperationFailure.Broadcast();
}
#pragma endregion

#pragma region Profile
void UPlayFabManager::GetPlayerProfile()
{
	PlayFab::ClientModels::FGetPlayerProfileRequest request;

	clientAPI->GetPlayerProfile(request,
		PlayFab::UPlayFabClientAPI::FGetPlayerProfileDelegate::CreateUObject(this, &UPlayFabManager::OnGetPlayerProfileSuccess),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UPlayFabManager::OnGetPlayerProfileError)
	);
}

void UPlayFabManager::OnGetPlayerProfileSuccess(const PlayFab::ClientModels::FGetPlayerProfileResult& Result)
{
	if (OnOperationSuccess.IsBound())
		OnOperationSuccess.Broadcast();
}

void UPlayFabManager::OnGetPlayerProfileError(const PlayFab::FPlayFabCppError& ErrorResult)
{
	if (OnOperationFailure.IsBound())
		OnOperationFailure.Broadcast();
}
#pragma endregion

UWorld* UPlayFabManager::GetWorld() const
{
	return UMiscFunctionLibrary::GetWorldReference();
}

bool UPlayFabManager::HandleErrorCode(const int32 errorCode)
{
	bool bIsHandled = false;
	switch (errorCode) {
	case 1000:
	{
		// InvalidParams : The API request object sent to PlayFab has invalid parameters and cannot be executed.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidParams : The API request object sent to server has invalid parameters and cannot be executed"));

		bIsHandled = true;
		break;
	}
	case 1001:
	{
		// AccountNotFound : The player account does not exist, likely because you are not copying a PlayFabId / TitlePlayerId correctly.This error will always occur if the identifier is not correct.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("The player account does not exist."));

		bIsHandled = true;
		break;
	}
	case 1002:
	{
		// AccountBanned : The player account has been banned, all API methods will fail with this error.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("The player account has been banned."));

		bIsHandled = true;
		break;
	}
	case 1003:
	{
		// InvalidUsernameOrPassword
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid username or password"));

		bIsHandled = true;
		break;
	}
	case 1004:
	{
		// InvalidTitleId : The request provided a TitleId which does not match the title provided in the URL of the method.In most SDKs, you should not specify a TitleId
		// for login requests, as it is done for you.In the admin API, explicit TitleIds are a Dev->Test->Live safety feature.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Title Id"));

		bIsHandled = true;
		break;
	}
	case 1005:
	{
		// InvalidEmailAddress 
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Email Address"));

		bIsHandled = true;
		break;
	}
	case 1006:
	{
		// EmailAddressNotAvailable
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Email Address Not Available"));

		bIsHandled = true;
		break;
	}
	case 1007:
	{
		// InvalidUsername;
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Username"));

		bIsHandled = true;
		break;
	}
	case 1008:
	{
		// InvalidPassword
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Password"));

		bIsHandled = true;
		break;
	}
	case 1009:
	{
		// UsernameNotAvailable
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Username Not Available"));

		bIsHandled = true;
		break;
	}
	case 1010:
	{
		// InvalidSteamTicket
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Steam Ticket"));

		bIsHandled = true;
		break;
	}
	case 1011:
	{
		// AccountAlreadyLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Account Already Linked"));

		bIsHandled = true;
		break;
	}
	case 1012:
	{
		// LinkedAccountAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Linked Account Already Claimed"));

		bIsHandled = true;
		break;
	}
	case 1013:
	{
		// InvalidFacebookToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("Invalid Facebook Token"));

		bIsHandled = true;
		break;
	}
	case 1014:
	{
		// AccountNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("AccountNotLinked"));

		bIsHandled = true;
		break;
	}
	case 1026:
	{
		// InvalidGoogleToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidGoogleToken"));

		bIsHandled = true;
		break;
	}
	case 1033:
	{
		// PlayerNotInGame
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PlayerNotInGame"));

		bIsHandled = true;
		break;
	}
	case 1039:
	{
		// UnknownError : This typically occurs if bad information is sent to a third - party add - on, and our server experienced an unknown result or error
		// while interacting with external systems.To resolve this, experiment with your inputs, and try to determine if your inputs are invalid in some way.
		// Otherwise, report the error on the forums, with your titleId, the full request JSON(if possible), and the error output.Postman is a useful tool for
		// debugging this situation.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("UnknownError"));

		bIsHandled = true;
		break;
	}
	case 1058:
	{
		// NameNotAvailable 
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NameNotAvailable"));

		bIsHandled = true;
		break;
	}
	case 1068:
	{
		// BodyTooLarge
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("BodyTooLarge"));

		bIsHandled = true;
		break;
	}
	case 1071:
	{
		// InvalidRequest : The API request object sent to PlayFab is invalid and cannot be executed.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidRequest"));

		bIsHandled = true;
		break;
	}
	case 1074:
	{
		// NotAuthenticated : The client has tried to call an API that requires SessionTicket authentication, without logging in first.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NotAuthenticated"));

		bIsHandled = true;
		break;
	}
	case 1089:
	{
		// NotAuthorized : Incorrect credentials, or otherwise bad inputs related to logging in.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NotAuthorized"));

		bIsHandled = true;
		break;
	}
	case 1092:
	{
		// InvalidPSNAuthCode
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidPSNAuthCode"));

		bIsHandled = true;
		break;
	}
	case 1118:
	{
		// LinkedDeviceAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("LinkedDeviceAlreadyClaimed"));

		bIsHandled = true;
		break;
	}
	case 1120:
	{
		// DeviceNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("DeviceNotLinked"));

		bIsHandled = true;
		break;
	}
	case 1123:
	{
		// ServiceUnavailable : Indicates that PlayFab may be having a temporary issue or the client is making too many API calls too quickly.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("ServiceUnavailable"));

		bIsHandled = true;
		break;
	}
	case 1127:
	{
		// DownstreamServiceUnavailable : Indicates that PlayFab or a third - party service might be having a temporary issue.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("DownstreamServiceUnavailable"));

		bIsHandled = true;
		break;
	}
	case 1131:
	{
		// InvalidAPIEndpoint : Indicates that the URL for this request is not valid for this title.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidAPIEndpoint"));

		bIsHandled = true;
		break;
	}
	case 1133:
	{
		// ConcurrentEditError : Indicates too many simultaneous calls or very rapid sequential calls.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("ConcurrentEditError"));

		bIsHandled = true;
		break;
	}
	case 1135:
	{
		// CharacterNotFound
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("CharacterNotFound"));

		bIsHandled = true;
		break;
	}
	case 1139:
	{
		// PhotonNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PhotonNotEnabledForTitle"));

		bIsHandled = true;
		break;
	}
	case 1140:
	{
		// PhotonApplicationNotFound
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PhotonApplicationNotFound"));

		bIsHandled = true;
		break;
	}
	case 1141:
	{
		// PhotonApplicationNotAssociatedWithTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PhotonApplicationNotAssociatedWithTitle"));

		bIsHandled = true;
		break;
	}
	case 1142:
	{
		// InvalidEmailOrPassword
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidEmailOrPassword"));

		bIsHandled = true;
		break;
	}
	case 1143:
	{
		// FacebookAPIError
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("FacebookAPIError"));

		bIsHandled = true;
		break;
	}
	case 1144:
	{
		// InvalidContentType : It should be impossible to get this if you're using one of our SDKs. If you're making your own raw HTTPS calls to PlayFab API methods, your Content - Type header must be application / json.No other formats are accepted.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidContentType"));

		bIsHandled = true;
		break;
	}
	case 1151:
	{
		// InvalidPSNIssuerId
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidPSNIssuerId"));

		bIsHandled = true;
		break;
	}
	case 1152:
	{
		// PSNInaccessible
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PSNInaccessible"));

		bIsHandled = true;
		break;
	}
	case 1176:
	{
		// InvalidKongregateToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidKongregateToken"));

		bIsHandled = true;
		break;
	}
	case 1177:
	{
		// FeatureNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("FeatureNotConfiguredForTitle"));

		bIsHandled = true;
		break;
	}
	case 1184:
	{
		// LinkedIdentifierAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("LinkedIdentifierAlreadyClaimed"));

		bIsHandled = true;
		break;
	}
	case 1185:
	{
		// CustomIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("CustomIdNotLinked"));

		bIsHandled = true;
		break;
	}
	case 1188:
	{
		// InvalidXboxLiveToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidXboxLiveToken"));

		bIsHandled = true;
		break;
	}
	case 1189:
	{
		// ExpiredXboxLiveToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("ExpiredXboxLiveToken"));

		bIsHandled = true;
		break;
	}
	case 1191:
	{
		// NotAuthorizedByTitle : This method has been disabled by the API Policy, and cannot be called.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NotAuthorizedByTitle"));

		bIsHandled = true;
		break;
	}
	case 1192:
	{
		// NoPartnerEnabled
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NoPartnerEnabled"));

		bIsHandled = true;
		break;
	}
	case 1193:
	{
		// InvalidPartnerResponse
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidPartnerResponse"));

		bIsHandled = true;
		break;
	}
	case 1199:
	{
		// APIClientRequestRateLimitExceeded : Indicates too many calls in a short burst.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("APIClientRequestRateLimitExceeded"));

		bIsHandled = true;
		break;
	}
	case 1207:
	{
		// NoWritePermissionsForEvent
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NoWritePermissionsForEvent"));

		bIsHandled = true;
		break;
	}
	case 1214:
	{
		// OverLimit : Indicates that an attempt to perform an operation will cause the service usage to go over the limit as shown in the Game Manager limit pages.Evaluate the returned error details to determine which limit would be exceeded.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("OverLimit"));

		bIsHandled = true;
		break;
	}
	case 1216:
	{
		// InvalidEventField
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidEventField"));

		bIsHandled = true;
		break;
	}
	case 1232:
	{
		// InvalidTwitchToken
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidTwitchToken"));

		bIsHandled = true;
		break;
	}
	case 1233:
	{
		// TwitchResponseError
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("TwitchResponseError"));

		bIsHandled = true;
		break;
	}
	case 1234:
	{
		// ProfaneDisplayName
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("ProfaneDisplayName"));

		bIsHandled = true;
		break;
	}
	case 1238:
	{
		// IdentifierAlreadyClaimed 
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("IdentifierAlreadyClaimed"));

		bIsHandled = true;
		break;
	}
	case 1239:
	{
		// IdentifierNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("IdentifierNotLinked"));

		bIsHandled = true;
		break;
	}
	case 1258:
	{
		// SteamNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("SteamNotEnabledForTitle"));

		bIsHandled = true;
		break;
	}
	case 1263:
	{
		// InvalidIdentityProviderId
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidIdentityProviderId"));

		bIsHandled = true;
		break;
	}
	case 1270:
	{
		// GoogleOAuthNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("GoogleOAuthNotConfiguredForTitle"));

		bIsHandled = true;
		break;
	}
	case 1271:
	{
		// GoogleOAuthError
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("GoogleOAuthError"));

		bIsHandled = true;
		break;
	}
	case 1273:
	{
		// InvalidSignature
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidSignature"));

		bIsHandled = true;
		break;
	}
	case 1274:
	{
		// InvalidPublicKey
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidPublicKey"));

		bIsHandled = true;
		break;
	}
	case 1275:
	{
		// GoogleOAuthNoIdTokenIncludedInResponse
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("GoogleOAuthNoIdTokenIncludedInResponse"));

		bIsHandled = true;
		break;
	}
	case 1287:
	{
		// DataUpdateRateExceeded : Indicates too many simultaneous calls, or very rapid sequential calls.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("DataUpdateRateExceeded"));

		bIsHandled = true;
		break;
	}
	case 1290:
	{
		// EncryptionKeyMissing
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("EncryptionKeyMissing"));

		bIsHandled = true;
		break;
	}
	case 1292:
	{
		// NoSharedSecretKeyConfigured
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NoSharedSecretKeyConfigured"));

		bIsHandled = true;
		break;
	}
	case 1294:
	{
		// PlayerSecretAlreadyConfigured
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PlayerSecretAlreadyConfigured"));

		bIsHandled = true;
		break;
	}
	case 1295:
	{
		// APIRequestsDisabledForTitle : All API requests have been disabled for this title, and it can no longer be used.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("APIRequestsDisabledForTitle"));

		bIsHandled = true;
		break;
	}
	case 1296:
	{
		// InvalidSharedSecretKey
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidSharedSecretKey"));

		bIsHandled = true;
		break;
	}
	case 1298:
	{
		// ProfileDoesNotExist : Attempted to access an entity(player, character, title, etc.), which does not exist.Probably a typo, or you've got a bad input somewhere.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("ProfileDoesNotExist"));

		bIsHandled = true;
		break;
	}
	case 1302:
	{
		// SignedRequestNotAllowed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("SignedRequestNotAllowed"));

		bIsHandled = true;
		break;
	}
	case 1303:
	{
		// RequestViewConstraintParamsNotAllowed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("RequestViewConstraintParamsNotAllowed"));

		bIsHandled = true;
		break;
	}
	case 1306:
	{
		// XboxXASSExchangeFailure
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("XboxXASSExchangeFailure"));

		bIsHandled = true;
		break;
	}
	case 1322:
	{
		// AccountDeleted : The player account has been deleted, all API methods will fail with this error.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("AccountDeleted"));

		bIsHandled = true;
		break;
	}
	case 1323:
	{
		// PlayerSecretNotConfigured
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("PlayerSecretNotConfigured"));

		bIsHandled = true;
		break;
	}
	case 1324:
	{
		// InvalidSignatureTime
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidSignatureTime"));

		bIsHandled = true;
		break;
	}
	case 1325:
	{
		// NoContactEmailAddressFound
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NoContactEmailAddressFound"));

		bIsHandled = true;
		break;
	}
	case 1339:
	{
		// XboxInaccessible
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("XboxInaccessible"));

		bIsHandled = true;
		break;
	}
	case 1341:
	{
		// SmtpAddonNotEnabled
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("SmtpAddonNotEnabled"));

		bIsHandled = true;
		break;
	}
	case 1342:
	{
		// APIConcurrentRequestLimitExceeded : Indicates too many simultaneous calls.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("APIConcurrentRequestLimitExceeded"));

		bIsHandled = true;
		break;
	}
	case 1343:
	{
		// XboxRejectedXSTSExchangeRequest
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("XboxRejectedXSTSExchangeRequest"));

		bIsHandled = true;
		break;
	}
	case 1347:
	{
		// TitleDeleted : This title has been deleted from PlayFab, and can no longer be used.
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("TitleDeleted"));

		bIsHandled = true;
		break;
	}
	case 1395:
	{
		// FacebookInstantGamesIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("FacebookInstantGamesIdNotLinked"));

		bIsHandled = true;
		break;
	}
	case 1396:
	{
		// InvalidFacebookInstantGamesSignature
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidFacebookInstantGamesSignature"));

		bIsHandled = true;
		break;
	}
	case 1397:
	{
		// FacebookInstantGamesAuthNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("FacebookInstantGamesAuthNotConfiguredForTitle"));

		bIsHandled = true;
		break;
	}
	case 1427:
	{
		// EmailRecipientBlacklisted
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("EmailRecipientBlacklisted"));

		bIsHandled = true;
		break;
	}
	case 1428:
	{
		// InvalidGameCenterAuthRequest
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("InvalidGameCenterAuthRequest"));

		bIsHandled = true;
		break;
	}
	case 1429:
	{
		// GameCenterAuthenticationFailed
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("GameCenterAuthenticationFailed"));

		bIsHandled = true;
		break;
	}
	case 1490:
	{
		// EvaluationModePlayerCountExceeded
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("EvaluationModePlayerCountExceeded"));

		bIsHandled = true;
		break;
	}
	case 1501:
	{
		// AppleNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("AppleNotEnabledForTitle"));

		bIsHandled = true;
		break;
	}
	case 2034:
	{
		// NintendoSwitchDeviceIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, TEXT("NintendoSwitchDeviceIdNotLinked"));

		bIsHandled = true;
		break;
	}

	default:
		break;
	}
	return bIsHandled;
}

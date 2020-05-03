// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "PlayFabManager.h"
#include "../TCWMobile.h"
#include "SystemFunctionLibrary.h"
#include "MiscFunctionLibrary.h"

#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "PlayFabClientModels.h"

// Define the namespace to use with LOCTEXT
// This is only valid within a single file, and must be undefined before the end of the file
#define LOCTEXT_NAMESPACE "PlayFabManager"
// Create text literals
const FText SomethingWrong = LOCTEXT("PlayFabManager_SomethingWrong", "Something went wrong.");
const FText NotImplemented = LOCTEXT("PlayFabManager_NotImplemented", "Not Implemented");

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
	// Retrieve and cast the USaveGame object to UAccountData.
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
		USystemFunctionLibrary::DisplayError(this, LOCTEXT("PlayFabManager_Reinstall", "Invalid stored account.\nPlease reinstall the game.").ToString());
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
		USystemFunctionLibrary::DisplayError(this, SomethingWrong.ToString());
#endif
	}
	else if (accountData->AccountType == EAccountType::AccountType_Facebook)
	{
		//TODO: FACEBOOK LOGIN
		USystemFunctionLibrary::DisplayError(this, NotImplemented.ToString());
	}
	else if (accountData->AccountType == EAccountType::AccountType_Apple)
	{
#if PLATFORM_IOS
		DeviceLogin();
#else
		USystemFunctionLibrary::DisplayError(this, SomethingWrong.ToString());
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
		accountData = Cast<UAccountData>(UGameplayStatics::CreateSaveGameObject(UAccountData::StaticClass()));

		// Set data on the savegame object.
		accountData->UserName = FPlatformMisc::GetDeviceId();
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
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidParams", "API request invalid params.").ToString());

		bIsHandled = true;
		break;
	}
	case 1001:
	{
		// AccountNotFound : The player account does not exist, likely because you are not copying a PlayFabId / TitlePlayerId correctly.This error will always occur if the identifier is not correct.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AccountNotFound", "The player account does not exist.").ToString());

		bIsHandled = true;
		break;
	}
	case 1002:
	{
		// AccountBanned : The player account has been banned, all API methods will fail with this error.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AccountBanned", "The player account has been banned.").ToString());

		bIsHandled = true;
		break;
	}
	case 1003:
	{
		// InvalidUsernameOrPassword
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidUsernameOrPassword", "Invalid username or password.").ToString());

		bIsHandled = true;
		break;
	}
	case 1004:
	{
		// InvalidTitleId : The request provided a TitleId which does not match the title provided in the URL of the method.In most SDKs, you should not specify a TitleId
		// for login requests, as it is done for you.In the admin API, explicit TitleIds are a Dev->Test->Live safety feature.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidTitleId", "Invalid Title Id.").ToString());

		bIsHandled = true;
		break;
	}
	case 1005:
	{
		// InvalidEmailAddress 
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidEmailAddress", "Invalid Email Address.").ToString());

		bIsHandled = true;
		break;
	}
	case 1006:
	{
		// EmailAddressNotAvailable
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_EmailAddressNotAvailable", "Email Address Not Available.").ToString());

		bIsHandled = true;
		break;
	}
	case 1007:
	{
		// InvalidUsername;
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidUsername", "Invalid Username.").ToString());

		bIsHandled = true;
		break;
	}
	case 1008:
	{
		// InvalidPassword
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidPassword", "Invalid Password.").ToString());

		bIsHandled = true;
		break;
	}
	case 1009:
	{
		// UsernameNotAvailable
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_UsernameNotAvailable", "Username Not Available.").ToString());

		bIsHandled = true;
		break;
	}
	case 1010:
	{
		// InvalidSteamTicket
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidSteamTicket", "Invalid Steam Ticket.").ToString());

		bIsHandled = true;
		break;
	}
	case 1011:
	{
		// AccountAlreadyLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AccountAlreadyLinked", "Account Already Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1012:
	{
		// LinkedAccountAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_LinkedAccountAlreadyClaimed", "Linked Account Already Claimed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1013:
	{
		// InvalidFacebookToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidFacebookToken", "Invalid Facebook Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1014:
	{
		// AccountNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AccountNotLinked", "Account Not Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1026:
	{
		// InvalidGoogleToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidGoogleToken", "Invalid Google Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1033:
	{
		// PlayerNotInGame
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PlayerNotInGame", "Player Not In Game.").ToString());

		bIsHandled = true;
		break;
	}
	case 1039:
	{
		// UnknownError : This typically occurs if bad information is sent to a third - party add - on, and our server experienced an unknown result or error
		// while interacting with external systems.To resolve this, experiment with your inputs, and try to determine if your inputs are invalid in some way.
		// Otherwise, report the error on the forums, with your titleId, the full request JSON(if possible), and the error output.Postman is a useful tool for
		// debugging this situation.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_UnknownError", "Unknown Error.").ToString());

		bIsHandled = true;
		break;
	}
	case 1058:
	{
		// NameNotAvailable 
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NameNotAvailable", "Name Not Available.").ToString());

		bIsHandled = true;
		break;
	}
	case 1068:
	{
		// BodyTooLarge
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_BodyTooLarge", "Body Too Large.").ToString());

		bIsHandled = true;
		break;
	}
	case 1071:
	{
		// InvalidRequest : The API request object sent to PlayFab is invalid and cannot be executed.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidRequest", "Invalid Request.").ToString());

		bIsHandled = true;
		break;
	}
	case 1074:
	{
		// NotAuthenticated : The client has tried to call an API that requires SessionTicket authentication, without logging in first.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NotAuthenticated", "Not Authenticated.").ToString());

		bIsHandled = true;
		break;
	}
	case 1089:
	{
		// NotAuthorized : Incorrect credentials, or otherwise bad inputs related to logging in.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NotAuthorized", "Not Authorized.").ToString());

		bIsHandled = true;
		break;
	}
	case 1092:
	{
		// InvalidPSNAuthCode
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidPSNAuthCode", "Invalid PSN Auth Code.").ToString());

		bIsHandled = true;
		break;
	}
	case 1118:
	{
		// LinkedDeviceAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_LinkedDeviceAlreadyClaimed", "Linked Device Already Claimed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1120:
	{
		// DeviceNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_DeviceNotLinked", "Device Not Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1123:
	{
		// ServiceUnavailable : Indicates that PlayFab may be having a temporary issue or the client is making too many API calls too quickly.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_ServiceUnavailable", "Service Unavailable.").ToString());

		bIsHandled = true;
		break;
	}
	case 1127:
	{
		// DownstreamServiceUnavailable : Indicates that PlayFab or a third - party service might be having a temporary issue.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_DownstreamServiceUnavailable", "Downstream Service Unavailable.").ToString());

		bIsHandled = true;
		break;
	}
	case 1131:
	{
		// InvalidAPIEndpoint : Indicates that the URL for this request is not valid for this title.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidAPIEndpoint", "Invalid API Endpoint.").ToString());

		bIsHandled = true;
		break;
	}
	case 1133:
	{
		// ConcurrentEditError : Indicates too many simultaneous calls or very rapid sequential calls.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_ConcurrentEditError", "Concurrent Edit Error.").ToString());

		bIsHandled = true;
		break;
	}
	case 1135:
	{
		// CharacterNotFound
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_CharacterNotFound", "Character Not Found.").ToString());

		bIsHandled = true;
		break;
	}
	case 1139:
	{
		// PhotonNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PhotonNotEnabledForTitle", "Photon Not Enabled For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1140:
	{
		// PhotonApplicationNotFound
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PhotonApplicationNotFound", "Photon Application Not Found.").ToString());

		bIsHandled = true;
		break;
	}
	case 1141:
	{
		// PhotonApplicationNotAssociatedWithTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PhotonApplicationNotAssociatedWithTitle", "Photon Application Not Associated With Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1142:
	{
		// InvalidEmailOrPassword
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidEmailOrPassword", "Invalid Email Or Password.").ToString());

		bIsHandled = true;
		break;
	}
	case 1143:
	{
		// FacebookAPIError
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_FacebookAPIError", "Facebook API Error.").ToString());

		bIsHandled = true;
		break;
	}
	case 1144:
	{
		// InvalidContentType : It should be impossible to get this if you're using one of our SDKs. If you're making your own raw HTTPS calls to PlayFab API methods, your Content - Type header must be application / json.No other formats are accepted.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidContentType", "Invalid Content Type.").ToString());

		bIsHandled = true;
		break;
	}
	case 1151:
	{
		// InvalidPSNIssuerId
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidPSNIssuerId", "Invalid PSN Issuer Id.").ToString());

		bIsHandled = true;
		break;
	}
	case 1152:
	{
		// PSNInaccessible
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PSNInaccessible", "PSN Inaccessible.").ToString());

		bIsHandled = true;
		break;
	}
	case 1176:
	{
		// InvalidKongregateToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidKongregateToken", "Invalid Kongregate Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1177:
	{
		// FeatureNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_FeatureNotConfiguredForTitle", "Feature Not Configured For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1184:
	{
		// LinkedIdentifierAlreadyClaimed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_LinkedIdentifierAlreadyClaimed", "Linked Identifier Already Claimed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1185:
	{
		// CustomIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_CustomIdNotLinked", "Custom Id Not Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1188:
	{
		// InvalidXboxLiveToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidXboxLiveToken", "Invalid Xbox Live Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1189:
	{
		// ExpiredXboxLiveToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_ExpiredXboxLiveToken", "Expired Xbox Live Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1191:
	{
		// NotAuthorizedByTitle : This method has been disabled by the API Policy, and cannot be called.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NotAuthorizedByTitle", "Not Authorized By Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1192:
	{
		// NoPartnerEnabled
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NoPartnerEnabled", "No Partner Enabled.").ToString());

		bIsHandled = true;
		break;
	}
	case 1193:
	{
		// InvalidPartnerResponse
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidPartnerResponse", "Invalid Partner Response.").ToString());

		bIsHandled = true;
		break;
	}
	case 1199:
	{
		// APIClientRequestRateLimitExceeded : Indicates too many calls in a short burst.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_APIClientRequestRateLimitExceeded", "API Client Request Rate Limit Exceeded.").ToString());

		bIsHandled = true;
		break;
	}
	case 1207:
	{
		// NoWritePermissionsForEvent
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NoWritePermissionsForEvent", "No Write Permissions For Event.").ToString());

		bIsHandled = true;
		break;
	}
	case 1214:
	{
		// OverLimit : Indicates that an attempt to perform an operation will cause the service usage to go over the limit as shown in the Game Manager limit pages.Evaluate the returned error details to determine which limit would be exceeded.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_OverLimit", "OverLimit.").ToString());

		bIsHandled = true;
		break;
	}
	case 1216:
	{
		// InvalidEventField
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidEventField", "Invalid Event Field.").ToString());

		bIsHandled = true;
		break;
	}
	case 1232:
	{
		// InvalidTwitchToken
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidTwitchToken", "Invalid Twitch Token.").ToString());

		bIsHandled = true;
		break;
	}
	case 1233:
	{
		// TwitchResponseError
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_TwitchResponseError", "Twitch Response Error.").ToString());

		bIsHandled = true;
		break;
	}
	case 1234:
	{
		// ProfaneDisplayName
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_ProfaneDisplayName", "Profane Display Name.").ToString());

		bIsHandled = true;
		break;
	}
	case 1238:
	{
		// IdentifierAlreadyClaimed 
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_IdentifierAlreadyClaimed", "Identifier Already Claimed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1239:
	{
		// IdentifierNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_IdentifierNotLinked", "Identifier Not Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1258:
	{
		// SteamNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_SteamNotEnabledForTitle", "Steam Not Enabled For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1263:
	{
		// InvalidIdentityProviderId
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidIdentityProviderId", "Invalid Identity Provider Id.").ToString());

		bIsHandled = true;
		break;
	}
	case 1270:
	{
		// GoogleOAuthNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_GoogleOAuthNotConfiguredForTitle", "Google OAuth Not Configured For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1271:
	{
		// GoogleOAuthError
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_GoogleOAuthError", "Google OAuth Error.").ToString());

		bIsHandled = true;
		break;
	}
	case 1273:
	{
		// InvalidSignature
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidSignature", "Invalid Signature.").ToString());

		bIsHandled = true;
		break;
	}
	case 1274:
	{
		// InvalidPublicKey
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidPublicKey", "Invalid Public Key.").ToString());

		bIsHandled = true;
		break;
	}
	case 1275:
	{
		// GoogleOAuthNoIdTokenIncludedInResponse
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_GoogleOAuthNoIdTokenIncludedInResponse", "Google OAuth No Id Token Included In Response.").ToString());

		bIsHandled = true;
		break;
	}
	case 1287:
	{
		// DataUpdateRateExceeded : Indicates too many simultaneous calls, or very rapid sequential calls.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_DataUpdateRateExceeded", "Data Update Rate Exceeded.").ToString());

		bIsHandled = true;
		break;
	}
	case 1290:
	{
		// EncryptionKeyMissing
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_EncryptionKeyMissing", "Encryption Key Missing.").ToString());

		bIsHandled = true;
		break;
	}
	case 1292:
	{
		// NoSharedSecretKeyConfigured
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NoSharedSecretKeyConfigured", "No Shared Secret Key Configured.").ToString());

		bIsHandled = true;
		break;
	}
	case 1294:
	{
		// PlayerSecretAlreadyConfigured
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PlayerSecretAlreadyConfigured", "Player Secret Already Configured.").ToString());

		bIsHandled = true;
		break;
	}
	case 1295:
	{
		// APIRequestsDisabledForTitle : All API requests have been disabled for this title, and it can no longer be used.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_APIRequestsDisabledForTitle", "API Requests Disabled For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1296:
	{
		// InvalidSharedSecretKey
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidSharedSecretKey", "Invalid Shared Secret Key.").ToString());

		bIsHandled = true;
		break;
	}
	case 1298:
	{
		// ProfileDoesNotExist : Attempted to access an entity(player, character, title, etc.), which does not exist.Probably a typo, or you've got a bad input somewhere.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_ProfileDoesNotExist", "Profile Does Not Exist.").ToString());

		bIsHandled = true;
		break;
	}
	case 1302:
	{
		// SignedRequestNotAllowed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_SignedRequestNotAllowed", "Signed Request Not Allowed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1303:
	{
		// RequestViewConstraintParamsNotAllowed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_RequestViewConstraintParamsNotAllowed", "Request View Constraint Params Not Allowed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1306:
	{
		// XboxXASSExchangeFailure
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_XboxXASSExchangeFailure", "Xbox XASS Exchange Failure.").ToString());

		bIsHandled = true;
		break;
	}
	case 1322:
	{
		// AccountDeleted : The player account has been deleted, all API methods will fail with this error.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AccountDeleted", "Account Deleted.").ToString());

		bIsHandled = true;
		break;
	}
	case 1323:
	{
		// PlayerSecretNotConfigured
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_PlayerSecretNotConfigured", "Player Secret Not Configured.").ToString());

		bIsHandled = true;
		break;
	}
	case 1324:
	{
		// InvalidSignatureTime
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidSignatureTime", "Invalid Signature Time.").ToString());

		bIsHandled = true;
		break;
	}
	case 1325:
	{
		// NoContactEmailAddressFound
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NoContactEmailAddressFound", "No Contact Email Address Found.").ToString());

		bIsHandled = true;
		break;
	}
	case 1339:
	{
		// XboxInaccessible
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_XboxInaccessible", "XboxInaccessible.").ToString());

		bIsHandled = true;
		break;
	}
	case 1341:
	{
		// SmtpAddonNotEnabled
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_SmtpAddonNotEnabled", "Smtp Addon Not Enabled.").ToString());

		bIsHandled = true;
		break;
	}
	case 1342:
	{
		// APIConcurrentRequestLimitExceeded : Indicates too many simultaneous calls.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_APIConcurrentRequestLimitExceeded", "API Concurrent Request Limit Exceeded.").ToString());

		bIsHandled = true;
		break;
	}
	case 1343:
	{
		// XboxRejectedXSTSExchangeRequest
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_XboxRejectedXSTSExchangeRequest", "Xbox Rejected XSTS Exchange Request.").ToString());

		bIsHandled = true;
		break;
	}
	case 1347:
	{
		// TitleDeleted : This title has been deleted from PlayFab, and can no longer be used.
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_TitleDeleted", "Title Deleted.").ToString());

		bIsHandled = true;
		break;
	}
	case 1395:
	{
		// FacebookInstantGamesIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_FacebookInstantGamesIdNotLinked", "Facebook Instant Games Id Not Linked.").ToString());

		bIsHandled = true;
		break;
	}
	case 1396:
	{
		// InvalidFacebookInstantGamesSignature
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidFacebookInstantGamesSignature", "Invalid Facebook Instant Games Signature.").ToString());

		bIsHandled = true;
		break;
	}
	case 1397:
	{
		// FacebookInstantGamesAuthNotConfiguredForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_FacebookInstantGamesAuthNotConfiguredForTitle", "Facebook Instant Games Auth Not Configured For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 1427:
	{
		// EmailRecipientBlacklisted
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_EmailRecipientBlacklisted", "Email Recipient Blacklisted.").ToString());

		bIsHandled = true;
		break;
	}
	case 1428:
	{
		// InvalidGameCenterAuthRequest
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_InvalidGameCenterAuthRequest", "Invalid Game Center AuthRequest.").ToString());

		bIsHandled = true;
		break;
	}
	case 1429:
	{
		// GameCenterAuthenticationFailed
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_GameCenterAuthenticationFailed", "Game Center Authentication Failed.").ToString());

		bIsHandled = true;
		break;
	}
	case 1490:
	{
		// EvaluationModePlayerCountExceeded
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_EvaluationModePlayerCountExceeded", "Evaluation Mode Player Count Exceeded.").ToString());

		bIsHandled = true;
		break;
	}
	case 1501:
	{
		// AppleNotEnabledForTitle
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_AppleNotEnabledForTitle", "Apple Not Enabled For Title.").ToString());

		bIsHandled = true;
		break;
	}
	case 2034:
	{
		// NintendoSwitchDeviceIdNotLinked
		USystemFunctionLibrary::DisplayError(nullptr, LOCTEXT("PlayFabManager_Error_NintendoSwitchDeviceIdNotLinked", "Nintendo Switch DeviceId Not Linked.").ToString());

		bIsHandled = true;
		break;
	}

	default:
		break;
	}
	return bIsHandled;
}

// Undefine the namespace before the end of the file
#undef LOCTEXT_NAMESPACE

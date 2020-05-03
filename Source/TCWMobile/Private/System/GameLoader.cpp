// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "GameLoader.h"
#include "MiscFunctionLibrary.h"
#include "SystemFunctionLibrary.h"

// Define the namespace to use with LOCTEXT
// This is only valid within a single file, and must be undefined before the end of the file
#define LOCTEXT_NAMESPACE "GameLoader"
// Create text literals
//const FText AllPlayersIn = LOCTEXT("PlayerController_", "All players are in!");

UGameLoader::UGameLoader()
{
	//PRE - 10
	//EXE-12
	OnLoadBegin.AddDynamic(this, &UGameLoader::LoadBeginEvent);
}

UWorld* UGameLoader::GetWorld() const
{
	return UMiscFunctionLibrary::GetWorldReference();
}

void UGameLoader::LoadBeginEvent()
{
	ReportAdvance(0.0f, true);
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);

	//EXE-10
	if (!AccountManagerRef)
		AccountManagerRef = USystemFunctionLibrary::GetTCWGameInstance(this)->GetAccountManager();

	if (!AccountManagerRef->IsLoggedIn())
	{
		AccountManagerRef->OnOperationSuccess.AddUniqueDynamic(this, &UGameLoader::LoginSuccessCallback);
		AccountManagerRef->OnOperationFailure.AddUniqueDynamic(this, &UGameLoader::LoginFailureCallback);
		if (AccountManagerRef->HasStoredLogin())
		{
			AccountManagerRef->StoredLogin();
		}
		else
		{
#if PLATFORM_ANDROID || PLATFORM_IOS
			AccountManagerRef->DeviceLogin();
#else
			AccountManagerRef->AnonymousLogin();
#endif
	}
		ReportAdvance(0.1f, false);
}
	else
	{
		LoginSuccessCallback();
	}

}

void UGameLoader::LoginSuccessCallback()
{
	ClearCallbacks();
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);

	if (AccountManagerRef->IsLoggedIn())
	{
		AccountManagerRef->OnOperationSuccess.AddUniqueDynamic(this, &UGameLoader::GetPlayerStatsSuccessCallback);
		AccountManagerRef->OnOperationFailure.AddUniqueDynamic(this, &UGameLoader::GetPlayerStatsFailureCallback);

		AccountManagerRef->GetPlayerStats();

		ReportAdvance(0.1f, false);
	}
	else
	{
		LoadBeginEvent();
	}

}

void UGameLoader::LoginFailureCallback()
{
	ClearCallbacks();
	USystemFunctionLibrary::DisplayError(this, LOCTEXT("GameLoader_FatalLoginUser", "FATAL: FAILED TO LOGIN USER").ToString(), 5.0f);

	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);
	LoadBeginEvent();
}

void UGameLoader::GetPlayerStatsSuccessCallback()
{
	ClearCallbacks();
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);

	if (AccountManagerRef->IsLoggedIn())
	{
		AccountManagerRef->OnOperationSuccess.AddUniqueDynamic(this, &UGameLoader::GetPlayerProfileSuccessCallback);
		AccountManagerRef->OnOperationFailure.AddUniqueDynamic(this, &UGameLoader::GetPlayerProfileFailureCallback);

		AccountManagerRef->GetPlayerProfile();

		ReportAdvance(0.1f, false);
	}
	else
	{
		LoadBeginEvent();
	}
}

void UGameLoader::GetPlayerStatsFailureCallback()
{
	ClearCallbacks();
	USystemFunctionLibrary::DisplayError(this, LOCTEXT("GameLoader_FatalGetStats", "FATAL: FAILED TO GET PLAYER STATS").ToString(), 5.0f);
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);
}

void UGameLoader::GetPlayerProfileSuccessCallback()
{
	ClearCallbacks();
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);
	ReportAdvance(1.0f, true);
	if (OnLoadCompleted.IsBound())
		OnLoadCompleted.Broadcast();
}

void UGameLoader::GetPlayerProfileFailureCallback()
{
	ClearCallbacks();
	USystemFunctionLibrary::DisplayError(this, LOCTEXT("GameLoader_FatalGetProfile", "FATAL: FAILED TO GET PLAYER PROFILE").ToString(), 5.0f);
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);
}

void UGameLoader::ReportAdvance(float percent, bool absolute)
{
	if (OnReportAdvance.IsBound())
		OnReportAdvance.Broadcast(percent, !absolute);
}

// Undefine the namespace before the end of the file
#undef LOCTEXT_NAMESPACE

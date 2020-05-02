// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#include "GameLoader.h"
#include "MiscFunctionLibrary.h"
#include "SystemFunctionLibrary.h"

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
	/*//if (!GameLoader)
	//	GameLoader = USystemFunctionLibrary::GetTCWGameInstance(this)->GetGameLoader();;*/
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

	/*
	//PREPARE GAME LOADING THREADS
	//TGraphTask<LoaderThreading::FUserLoginTask>* userLoginTask = TGraphTask<LoaderThreading::FUserLoginTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndHold(AccountManagerRef);

	//FGraphEventArray userPre = FGraphEventArray();
	//userPre.Add(userLoginTask->GetCompletionEvent());

	//TGraphTask<LoaderThreading::FCheckUserTask>* checkUserTask = TGraphTask<LoaderThreading::FCheckUserTask>::CreateTask(&userPre, ENamedThreads::GameThread).ConstructAndHold(AccountManagerRef);

	//userLoginTask->GetCompletionEvent()->AddSubsequent(checkUserTask);
	//Multithread_CompletionEvents.Add(userLoginTask->GetCompletionEvent());
	//Multithread_CompletionEvents.Add(checkUserTask->GetCompletionEvent());

	//START GAME LOADING
	//Multithread_CompletionEvents.Add(TGraphTask<FVictoryTestTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady()); //add properties inside ConstructAndDispatchWhenReady()
	//userLoginTask->Unlock();
	*/
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
	USystemFunctionLibrary::DisplayError(this, TEXT("FATAL: FAILED TO LOGIN USER"), 5.0f);

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
	USystemFunctionLibrary::DisplayError(this, TEXT("FATAL: FAILED TO GET PLAYER STATS"), 5.0f);
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
	USystemFunctionLibrary::DisplayError(this, TEXT("FATAL: FAILED TO GET PLAYER PROFILE"), 5.0f);
	UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 5.0f);
}

void UGameLoader::ReportAdvance(float percent, bool absolute)
{
	if (OnReportAdvance.IsBound())
		OnReportAdvance.Broadcast(percent, !absolute);
}

/* Begin Multi Thread
//TArray<FBaseGraphTask*> UGameLoader::TaskGraphs = TArray<FBaseGraphTask*>();
FGraphEventArray UGameLoader::Multithread_CompletionEvents = FGraphEventArray();
UGameLoader* UGameLoader::GameLoader = NULL;

bool UGameLoader::TasksAreComplete()
{
	//Check all thread completion events
	for (int32 Index = 0; Index < Multithread_CompletionEvents.Num(); Index++)
	{
		//If  ! IsComplete()
		if (!Multithread_CompletionEvents[Index]->IsComplete())
		{
			return false;
		}
	}
	return true;
}


LoaderThreading::FUserLoginTask::FUserLoginTask(UPlayFabManager* accountManagerRef) : FGameLoaderTask(),
AccountManagerRef(accountManagerRef)
{
}

LoaderThreading::FUserLoginTask::~FUserLoginTask()
{
}

void LoaderThreading::FUserLoginTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (AccountManagerRef->HasStoredLogin())
	{
		AccountManagerRef->StoredLogin();
	}
	else
	{

	}
	ReportAdvance();
}

LoaderThreading::FCheckUserTask::FCheckUserTask(UPlayFabManager* accountManagerRef) : FGameLoaderTask(),
AccountManagerRef(accountManagerRef)
{
}

LoaderThreading::FCheckUserTask::~FCheckUserTask()
{
}

void LoaderThreading::FCheckUserTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (AccountManagerRef->IsLoggedIn())
	{
		UE_LOG(TCWLog, Log, TEXT("User Logged In"));
	}
	else
	{
		UE_LOG(TCWLog, Log, TEXT("User NOT Logged In"));
	}
	ReportAdvance();
}
/* End Multi Thread */

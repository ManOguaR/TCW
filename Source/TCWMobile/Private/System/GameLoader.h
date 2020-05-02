// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TCWMobile.h"
#include "PlayFabManager.h"

#include "GameLoader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameLoaderSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReportAdvanceSignature, float, Percent, bool, IsProgress);

class FGameLoaderWorker;

/**
 * Encapsulates code for game initialization and user account data loading
 */
UCLASS(BlueprintType)
class UGameLoader : public UObject
{
	GENERATED_BODY()

public:
	UGameLoader();

	UPROPERTY(BlueprintCallable, Category = "Game Loader Events")
		FGameLoaderSignature OnLoadBegin;

	UPROPERTY(BlueprintAssignable, Category = "Game Loader Events")
		FReportAdvanceSignature OnReportAdvance;

private:
	UPlayFabManager* AccountManagerRef = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Game Loader Events")
		FGameLoaderSignature OnLoadCompleted;

	// Begin UObject interface
	UWorld* GetWorld() const override;
	// End UObject interface

private:
	UFUNCTION()
		void LoadBeginEvent();

	UFUNCTION()
		void LoginSuccessCallback();
	UFUNCTION()
		void LoginFailureCallback();
	UFUNCTION()
		void GetPlayerStatsSuccessCallback();
	UFUNCTION()
		void GetPlayerStatsFailureCallback();
	UFUNCTION()
		void GetPlayerProfileSuccessCallback();
	UFUNCTION()
		void GetPlayerProfileFailureCallback();

	void ClearCallbacks()
	{
		AccountManagerRef->OnOperationSuccess.Clear();
		AccountManagerRef->OnOperationFailure.Clear();
	};

	void ReportAdvance(float percent, bool absolute);

public:
	/* Begin Multi Thread
	//Multi-threaded link to UObjects, do not create,modify,destroy UObjects / AActors via this link!
	//static UGameLoader* GameLoader;
	//static TArray<FBaseGraphTask*> TaskGraphs;
	//static FGraphEventArray Multithread_CompletionEvents;

	//~~~~~~~~~~~~~~~
	//Are All Tasks Complete?
	//~~~~~~~~~~~~~~~
	//static bool TasksAreComplete();

	//class FGameLoaderTask
	//{

	//public:
	//	FGameLoaderTask() { } //send in property defaults here
	//	virtual ~FGameLoaderTask() { }

	//	/** return the name of the task 
	//	static const TCHAR* GetTaskName() { return TEXT("FGameLoaderTask"); }

	//	FORCEINLINE static TStatId GetStatId() { RETURN_QUICK_DECLARE_CYCLE_STAT(FVictoryTestTask, STATGROUP_TaskGraphTasks); }

	//	/** return the thread for this task 
	//	static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::AnyThread; }

	//	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	//	//~~~~~~~~~~~~~~~~~~~~~~~~
	//	//Main Function: Do Task!!
	//	//~~~~~~~~~~~~~~~~~~~~~~~~
	//	virtual void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& completionGraphEvent) = 0;

	//	void ReportAdvance(float percent = 0.1f, bool absolute = false)
	//	{
	//		GameLoader->ReportAdvance(percent, absolute);
	//	}
	//};

	/* End Multi Thread */
};

//Multi thread Test, finding prime number
//namespace LoaderThreading
//{
//	class FUserLoginTask : public UGameLoader::FGameLoaderTask
//	{
//	private:
//		UPlayFabManager* AccountManagerRef = nullptr;
//
//	public:
//		FUserLoginTask(UPlayFabManager* accountManagerRef);
//		~FUserLoginTask();
//
//		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
//	};
//
//	class FCheckUserTask : public UGameLoader::FGameLoaderTask
//	{
//		UPlayFabManager* AccountManagerRef = nullptr;
//
//	public:
//		FCheckUserTask(UPlayFabManager* accountManagerRef);
//		~FCheckUserTask();
//
//		void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
//	};
//}
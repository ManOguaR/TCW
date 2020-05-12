// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TCWMobile.h"
#include "PlayFabManager.h"

#include "GameLoader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameLoaderEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReportAdvanceEvent, float, Percent, bool, IsProgress);

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
		FGameLoaderEvent OnLoadBegin;

	UPROPERTY(BlueprintAssignable, Category = "Game Loader Events")
		FReportAdvanceEvent OnReportAdvance;

private:
	UPlayFabManager* AccountManagerRef = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Game Loader Events")
		FGameLoaderEvent OnLoadCompleted;

	// Begin UObject interface
	UWorld* GetWorld() const override;
	// End UObject interface

private:
	UFUNCTION()
		void LoadBegin();

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

};

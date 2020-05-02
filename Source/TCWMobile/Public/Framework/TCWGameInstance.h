// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"

#include "Enums.h"
#include "MenuEnums.h"
#include "SystemEnums.h"

#include "GameLoader.h"
#include "PlayFabManager.h"
#include "SessionManager.h"

#include "TCWGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWGameInstanceSignature);

/**
 *
 */
UCLASS()
class TCWMOBILE_API UTCWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTCWGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable, Category = "Startup Events")
		FTCWGameInstanceSignature OnStartup;
	UPROPERTY(BlueprintAssignable, Category = "Startup Events")
		FTCWGameInstanceSignature OnStartupSplashCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceSignature OnShowMainMenu;
	UPROPERTY(BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceSignature OnMainMenuSplashCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceSignature OnShowLoadingScreen;
	UPROPERTY(BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceSignature OnLoadingScreenSplashCompleted;

	FString SelectedCardSet;
public:
	UFUNCTION(BlueprintCallable, Category = "TCW Game System")
		EPlatform GetCurrentPlatform(bool& isMobile);

	UFUNCTION(BlueprintCallable, Category = "TCW Game System")
		EGameState GetGameState(EGameState inState, bool& sameState);

	UFUNCTION(BlueprintCallable, Category = "TCW Account")
		UPlayFabManager* GetAccountManager() const {
		return PlayFabManager;
	};
	UFUNCTION(BlueprintCallable, Category = "Game Session")
		USessionManager* GetSessionManager() const {
		return SessionManager;
	};
	UFUNCTION(BlueprintCallable, Category = "Game Startup")
		UGameLoader* GetGameLoader() const {
		return GameLoader;
	};

	void Init() override;

private:
	UFUNCTION()
		void StartupEvent();
	UFUNCTION()
		void ShowMainMenuEvent();
	UFUNCTION()
		void ShowLoadingScreenEvent();

	EGameState CurrentGameState;
	EPlatform Platform;
	TArray<FString> PlatformsArray = { "Windows", "iOS", "Android", "HTML5", "Mac", "Linux" };

	UPROPERTY()
		UPlayFabManager* PlayFabManager;
	UPROPERTY()
		USessionManager* SessionManager;
	UPROPERTY()
		UGameLoader* GameLoader;

	UUserWidget* MainMenuWidgetRef;
	UUserWidget* LoadingScreenWidgetRef;
	UUserWidget* DeckBuilderWidgetRef;
	UUserWidget* StartupWidgetRef;

	bool MoveToGameState(EGameState inState);
};
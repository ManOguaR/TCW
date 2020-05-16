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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWGameInstanceEvent);

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
		FTCWGameInstanceEvent OnStartup;
	UPROPERTY(BlueprintAssignable, Category = "Startup Events")
		FTCWGameInstanceEvent OnStartupSplashCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceEvent OnShowMainMenu;
	UPROPERTY(BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceEvent OnMainMenuSplashCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceEvent OnShowLoadingScreen;
	UPROPERTY(BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceEvent OnLoadingScreenSplashCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceEvent OnHostGameEvent;

	UPROPERTY(BlueprintReadWrite, Category = "TCW Game System")
		EGameState CurrentGameState;

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

private:
	UFUNCTION()
		void Startup();
	UFUNCTION()
		void ShowMainMenu();
	UFUNCTION()
		void ShowLoadingScreen();
	UFUNCTION()
		void HostGame();

	void HostGame_Continue();

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

public:
	void Init() final;

};
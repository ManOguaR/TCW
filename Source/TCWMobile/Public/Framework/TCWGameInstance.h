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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectionManagerEvent, FString, selectedDeck);

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
		FTCWGameInstanceEvent OnLoadMainMenu;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceEvent OnShowMainMenu;
	UPROPERTY(BlueprintAssignable, Category = "Menu Events")
		FTCWGameInstanceEvent OnShowMainMenuCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceEvent OnShowLoadingScreen;
	UPROPERTY(BlueprintAssignable, Category = "Loading Events")
		FTCWGameInstanceEvent OnLoadingScreenSplashCompleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Collection Manager Events")
		FCollectionManagerEvent OnLoadCollectionManager;
	UPROPERTY(BlueprintCallable, Category = "Collection Manager Events")
		FTCWGameInstanceEvent OnCollectionManagerLoaded;

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
		FCollectionManagerEvent OnHostGame;

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
		void LoadMainMenu();
	UFUNCTION()
		void ShowMainMenu();
	UFUNCTION()
		void ShowLoadingScreen();
	UFUNCTION()
		void LoadCollectionManager(FString selectedDeck);
	UFUNCTION()
		void CollectionManagerLoaded();
	UFUNCTION()
		void HostGame(FString selectedDeck);

	void HostGame_Continue();

	EPlatform Platform;
	TArray<FString> PlatformsArray = { "Windows", "iOS", "Android", "HTML5", "Mac", "Linux" };

	UPROPERTY()
		UPlayFabManager* PlayFabManager;
	UPROPERTY()
		USessionManager* SessionManager;
	UPROPERTY()
		UGameLoader* GameLoader;

public:
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* MainMenuWidgetRef;
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* LoadingScreenWidgetRef;
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* DeckBuilderWidgetRef;
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* StartupWidgetRef;

private:
	UFUNCTION()
		bool MoveToGameState(EGameState inState);

public:
	void Init() final;

};
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

protected:
	bool HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld) final;
	bool HandleDisconnectCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld) final;
	bool HandleReconnectCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld) final;
	bool HandleTravelCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld) final;

public:
	//~ Begin FExec Interface
	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Out = *GLog) final;
	//~ End FExec Interface

	//~ Begin UObject Interface
	void FinishDestroy() final;
	//~ End UObject Interface

	void Init() final;

	/** virtual function to allow custom GameInstances an opportunity to do cleanup when shutting down */
	void Shutdown() final;

#if WITH_EDITOR
	/* Called to initialize the game instance for PIE instances of the game */
	FGameInstancePIEResult InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params) final;
	/* Called to actually start the game when doing Play/Simulate In Editor */
	FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) final;
	/** Called as soon as the game mode is spawned, to allow additional PIE setting validation prior to creating the local players / etc... (called on pure clients too, in which case the game mode is nullptr) */
	FGameInstancePIEResult PostCreateGameModeForPIE(const FGameInstancePIEParameters& Params, AGameModeBase* GameMode) final;
#endif

	/** Callback from the world context when the world changes */
	void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) final { }

	/** Starts the GameInstance state machine running */
	void StartGameInstance() final;
	bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults) final;
	bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) final;

	void LoadComplete(const float LoadTime, const FString& MapName) final {}

	/**
	* Debug console command to create a player.
	* @param ControllerId - The controller ID the player should accept input from.
	*/
	void DebugCreatePlayer(int32 ControllerId) final;

	/**
	 * Debug console command to remove the player with a given controller ID.
	 * @param ControllerId - The controller ID to search for.
	 */
	void DebugRemovePlayer(int32 ControllerId) final;

	ULocalPlayer* CreateInitialPlayer(FString& OutError) final;

	/**
	 * Adds a LocalPlayer to the local and global list of Players.
	 *
	 * @param	NewPlayer	the player to add
	 * @param	ControllerId id of the controller associated with the player
	 */
	int32 AddLocalPlayer(ULocalPlayer* NewPlayer, int32 ControllerId) final;

	/**
	 * Removes a player.
	 * @param Player - The player to remove.
	 * @return whether the player was successfully removed. Removal is not allowed while connected to a server.
	 */
	bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) final;

	/** Called when demo playback fails for any reason */
	void HandleDemoPlaybackFailure(EDemoPlayFailure::Type FailureType, const FString& ErrorString = TEXT("")) final { }

	/** This gets called when the player scrubs in a replay to a different level */
	void OnSeamlessTravelDuringReplay() final { }

	/**
	* Start recording a replay with the given custom name and friendly name.
	*
	* @param InName If not empty, the unique name to use as an identifier for the replay. If empty, a name will be automatically generated by the replay streamer implementation.
	* @param FriendlyName An optional (may be empty) descriptive name for the replay. Does not have to be unique.
	* @param AdditionalOptions Additional URL options to append to the URL that will be processed by the replay net driver. Will usually remain empty.
	*/
	void StartRecordingReplay(const FString& InName, const FString& FriendlyName, const TArray<FString>& AdditionalOptions = TArray<FString>()) final;

	/** Stop recording a replay if one is currently in progress */
	void StopRecordingReplay() final;

	/**
	 * Start playing back a previously recorded replay.
	 *
	 * @param InName				Name of the replay file.
	 * @param WorldOverride			World in which the replay will be played. Passing null will cause the current world to be used.
	 * @param AdditionalOptions		Additional options that can be read by derived game instances, or the Demo Net Driver.
	 *
	 * @return True if the replay began successfully.
	 */
	bool PlayReplay(const FString& InName, UWorld* WorldOverride = nullptr, const TArray<FString>& AdditionalOptions = TArray<FString>()) final;

	/**
	* Adds a join-in-progress user to the set of users associated with the currently recording replay (if any)
	*
	* @param UserString a string that uniquely identifies the user, usually his or her FUniqueNetId
	*/
	void AddUserToReplay(const FString& UserString) final;

	/** handle a game specific net control message (NMT_GameSpecific)
	 * this allows games to insert their own logic into the control channel
	 * the meaning of both data parameters is game-specific
	 */
	void HandleGameNetControlMessage(class UNetConnection* Connection, uint8 MessageByte, const FString& MessageStr) final
	{}

	/** Handle setting up encryption keys. Games that override this MUST call the delegate when their own (possibly async) processing is complete. */
	void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) final;

	/** Called when a client receives the EncryptionAck control message from the server, will generally enable encryption. */
	void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) final;

	/** Call to preload any content before loading a map URL, used during seamless travel as well as map loading */
	void PreloadContentForURL(FURL InURL) final;

	/** Call to create the game mode for a given map URL */
	class AGameModeBase* CreateGameModeForURL(FURL InURL);

	/** Return the game mode subclass to use for a given map, options, and portal. By default return passed in one */
	TSubclassOf<AGameModeBase> OverrideGameModeClass(TSubclassOf<AGameModeBase> GameModeClass, const FString& MapName, const FString& Options, const FString& Portal) const final;

	/** return true to delay an otherwise ready-to-join PendingNetGame performing LoadMap() and finishing up
	 * useful to wait for content downloads, etc
	 */
	bool DelayPendingNetGameTravel() final
	{
		return false;
	}

	/** @return OnlineSession class to use for this game instance  */
	TSubclassOf<UOnlineSession> GetOnlineSessionClass() final;

	/**
	* Retrieves the name of the online subsystem for the platform used by this instance.
	* This will be used as the value of the PlayerOnlinePlatformName parameter in
	* the NMT_Login message when this client connects to a server.
	* Normally this will be the same as the DefaultPlatformService config value,
	* but games may override it if they need non-default behavior (for example,
	* if they are using multiple online subsystems at the same time).
	*/
	FName GetOnlinePlatformName() const final;

	/**
	 * Helper function for traveling to a session that has already been joined via the online platform
	 * Grabs the URL from the session info and travels
	 *
	 * @param ControllerId controller initiating the request
	 * @param InSessionName name of session to travel to
	 *
	 * @return true if able or attempting to travel, false otherwise
	 */
	bool ClientTravelToSession(int32 ControllerId, FName InSessionName) final;

	/**
	* Calls HandleDisconnect on either the OnlineSession if it exists or the engine, to cause a travel back to the default map. The instance must have a world.
	*/
	void ReturnToMainMenu() final;

	/** Registers an object to keep alive as long as this game instance is alive */
	void RegisterReferencedObject(UObject* ObjectToReference) final;

	/** Remove a referenced object, this will allow it to GC out */
	void UnregisterReferencedObject(UObject* ObjectToReference) final;

protected:
	/** Called when the game instance is started either normally or through PIE. */
	void OnStart() final;
};
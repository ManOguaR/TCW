// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWGameInstance.h"
#include "../TCWMobile.h"
#include "CreateSessionCallbackProxy.h"
#include "MiscFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

UTCWGameInstance::UTCWGameInstance(const FObjectInitializer& ObjectInitializer) : UGameInstance(ObjectInitializer)
{
	//Pre-20
	//EXE-1
	OnStartup.AddDynamic(this, &UTCWGameInstance::Startup);

	OnShowMainMenu.AddDynamic(this, &UTCWGameInstance::ShowMainMenu);
	OnShowLoadingScreen.AddDynamic(this, &UTCWGameInstance::ShowLoadingScreen);

	OnHostGameEvent.AddDynamic(this, &UTCWGameInstance::HostGame);

}

EPlatform UTCWGameInstance::GetCurrentPlatform(bool& isMobile)
{
	isMobile = (Platform == EPlatform::Platform_iOS || Platform == EPlatform::Platform_Android);

	return Platform;
}

EGameState UTCWGameInstance::GetGameState(EGameState inState, bool& sameSTate)
{
	sameSTate = inState == CurrentGameState;

	return CurrentGameState;
}

void UTCWGameInstance::Startup()
{
	//ENSURE STATE MACHINE
	if (MoveToGameState(EGameState::GameState_Startup))
		//../Game/Maps/MainMenu
		UGameplayStatics::OpenLevel(this, "MainMenu");

	if (GameLoader == nullptr)
		GameLoader = NewObject<UGameLoader>();

	//LOAD WIDGET IF COSMETICS ENABLED
	if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
	{
		if (!StartupWidgetRef->IsValidLowLevel())
		{
			//../Game/Blueprints/Widgets/MainMenu/StartupWidget.uasset
			FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/StartupWidget.StartupWidget_C"));
			if (UClass* widgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
			{
				StartupWidgetRef = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
			}
		}

		StartupWidgetRef->AddToViewport(5);
	}

	//DESTROY PENDING SESSIONS IF ANY
	if (SessionManager->HasActiveSession())
		SessionManager->DestroyPlayerSession(UGameplayStatics::GetPlayerController(this, 0));

	//SIGNAL STEP COMPLETED
	if (OnStartupSplashCompleted.IsBound())
		OnStartupSplashCompleted.Broadcast();
}

void UTCWGameInstance::ShowMainMenu()
{
	if (CurrentGameState == EGameState::GameState_Playing)
		//../Game/Maps/MainMenu
		UGameplayStatics::OpenLevel(this, "MainMenu");

	if (MoveToGameState(EGameState::GameState_MainMenu))
	{
		if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
		{
			if (!MainMenuWidgetRef->IsValidLowLevel())
			{
				//../Game/Blueprints/Widgets/MainMenu/MainMenuWidget.uasset
				FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/MainMenu/MainMenuWidget.MainMenuWidget_C"));
				if (UClass* widgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
				{
					MainMenuWidgetRef = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
				}
			}

			MainMenuWidgetRef->AddToViewport(5);
		}
	}

	//DESTROY PENDING SESSIONS IF ANY
	if (SessionManager->HasActiveSession())
		SessionManager->DestroyPlayerSession(UGameplayStatics::GetPlayerController(this, 0));

	//SIGNAL STEP COMPLETED
	if (OnMainMenuSplashCompleted.IsBound())
		OnMainMenuSplashCompleted.Broadcast();
}

void UTCWGameInstance::ShowLoadingScreen()
{
	MoveToGameState(EGameState::GameState_LoadingScreen);

	if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
	{
		if (!LoadingScreenWidgetRef->IsValidLowLevel())
		{
			FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/LoadingScreenWidget.LoadingScreenWidget_C"));
			if (UClass* widgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
			{
				LoadingScreenWidgetRef = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
			}
		}

		LoadingScreenWidgetRef->AddToViewport(5);
	}

	//SIGNAL STEP COMPLETED
	if (OnLoadingScreenSplashCompleted.IsBound())
		OnLoadingScreenSplashCompleted.Broadcast();
}

void UTCWGameInstance::HostGame()
{
	if (OnShowLoadingScreen.IsBound())
		OnShowLoadingScreen.Broadcast();

	UKismetSystemLibrary::Delay(this, 2.0, FLatentActionInfo());

	//UCreateSessionCallbackProxy* sessionProxy = UCreateSessionCallbackProxy::CreateSession(this, UGameplayStatics::GetPlayerController(this, 0), 3, true);

	//if(sessionProxy->OnSuccess)
}

bool UTCWGameInstance::MoveToGameState(EGameState inState)
{
	if (CurrentGameState != inState)
	{
		//DO STATE CLEANUP
		if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
		{
			switch (CurrentGameState)
			{
			case EGameState::GameState_MainMenu:
			{
				MainMenuWidgetRef->RemoveFromParent();
				break;
			}
			case EGameState::GameState_LoadingScreen:
			{
				LoadingScreenWidgetRef->RemoveFromParent();
				break;
			}
			case EGameState::GameState_DeckBuilding:
			{
				DeckBuilderWidgetRef->RemoveFromParent();
				break;
			}
			case EGameState::GameState_Startup:
			{
				if (GameLoader != nullptr)
					GameLoader = nullptr;

				StartupWidgetRef->RemoveFromParent();
				break;
			}
			case EGameState::GameState_Playing:
			case EGameState::GameState_Store:
			default:
				break;
			}
		}
		// SETUP NEW STATE
		CurrentGameState = inState;

		//State has changed
		return true;
	}

	//Unchanged state
	return false;
}

//OVERRIDES - FINAL
bool UTCWGameInstance::HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	return Super::HandleOpenCommand(Cmd, Ar, InWorld);
}
bool UTCWGameInstance::HandleDisconnectCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	return Super::HandleOpenCommand(Cmd, Ar, InWorld);
}
bool UTCWGameInstance::HandleReconnectCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	return Super::HandleOpenCommand(Cmd, Ar, InWorld);
}
bool UTCWGameInstance::HandleTravelCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	return Super::HandleOpenCommand(Cmd, Ar, InWorld);
}

bool UTCWGameInstance::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Out)
{
	return Super::Exec(InWorld, Cmd, Out);
}

void UTCWGameInstance::FinishDestroy()
{
	Super::FinishDestroy();
}

void UTCWGameInstance::Init()
{
	//EXE-2
	Super::Init();

	Platform = (EPlatform)(uint8)PlatformsArray.Find(UGameplayStatics::GetPlatformName());

	//Singleton-like initialization of system objects.
	if (PlayFabManager == nullptr)
		PlayFabManager = NewObject<UPlayFabManager>();
	if (SessionManager == nullptr)
		SessionManager = NewObject<USessionManager>();

	//Other global initializations here...
}

void UTCWGameInstance::Shutdown()
{
	Super::Shutdown();
}

#if WITH_EDITOR
FGameInstancePIEResult UTCWGameInstance::InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params)
{
	return Super::InitializeForPlayInEditor(PIEInstanceIndex, Params);
}
FGameInstancePIEResult UTCWGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	return Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
}
FGameInstancePIEResult UTCWGameInstance::PostCreateGameModeForPIE(const FGameInstancePIEParameters& Params, AGameModeBase* GameMode)
{
	return Super::PostCreateGameModeForPIE(Params, GameMode);
}
#endif

void UTCWGameInstance::StartGameInstance()
{
	Super::StartGameInstance();
}

bool UTCWGameInstance::JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults)
{
	return Super::JoinSession(LocalPlayer, SessionIndexInSearchResults);
}

bool UTCWGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	return Super::JoinSession(LocalPlayer, SearchResult);
}

void UTCWGameInstance::DebugCreatePlayer(int32 ControllerId)
{
	return Super::DebugCreatePlayer(ControllerId);
}

void UTCWGameInstance::DebugRemovePlayer(int32 ControllerId)
{
	return Super::DebugRemovePlayer(ControllerId);
}

ULocalPlayer* UTCWGameInstance::CreateInitialPlayer(FString& OutError)
{
	return Super::CreateInitialPlayer(OutError);
}

int32 UTCWGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, int32 ControllerId)
{
	return Super::AddLocalPlayer(NewPlayer, ControllerId);
}

bool UTCWGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UTCWGameInstance::StartRecordingReplay(const FString& InName, const FString& FriendlyName, const TArray<FString>& AdditionalOptions)
{
	Super::StartRecordingReplay(InName, FriendlyName, AdditionalOptions);
}

void UTCWGameInstance::StopRecordingReplay()
{
	Super::StopRecordingReplay();
}

bool UTCWGameInstance::PlayReplay(const FString& InName, UWorld* WorldOverride, const TArray<FString>& AdditionalOptions)
{
	return Super::PlayReplay(InName, WorldOverride, AdditionalOptions);
}

void UTCWGameInstance::AddUserToReplay(const FString& UserString)
{
	Super::AddUserToReplay(UserString);
}

void UTCWGameInstance::ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate)
{
	Super::ReceivedNetworkEncryptionToken(EncryptionToken, Delegate);
}

void UTCWGameInstance::ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate)
{
	Super::ReceivedNetworkEncryptionAck(Delegate);
}

void UTCWGameInstance::PreloadContentForURL(FURL InURL)
{
	Super::PreloadContentForURL(InURL);
}

AGameModeBase* UTCWGameInstance::CreateGameModeForURL(FURL InURL)
{
	return Super::CreateGameModeForURL(InURL);
}

TSubclassOf<AGameModeBase> UTCWGameInstance::OverrideGameModeClass(TSubclassOf<AGameModeBase> GameModeClass, const FString& MapName, const FString& Options, const FString& Portal) const
{
	return Super::OverrideGameModeClass(GameModeClass, MapName, Options, Portal);
}

TSubclassOf<UOnlineSession> UTCWGameInstance::GetOnlineSessionClass()
{
	return Super::GetOnlineSessionClass();
}

FName UTCWGameInstance::GetOnlinePlatformName() const
{
	return Super::GetOnlinePlatformName();
}

bool UTCWGameInstance::ClientTravelToSession(int32 ControllerId, FName InSessionName)
{
	return Super::ClientTravelToSession(ControllerId, InSessionName);
}

void UTCWGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}

void UTCWGameInstance::RegisterReferencedObject(UObject* ObjectToReference)
{
	Super::RegisterReferencedObject(ObjectToReference);
}

void UTCWGameInstance::UnregisterReferencedObject(UObject* ObjectToReference)
{
	Super::UnregisterReferencedObject(ObjectToReference);
}

void UTCWGameInstance::OnStart()
{
	Super::OnStart();
}

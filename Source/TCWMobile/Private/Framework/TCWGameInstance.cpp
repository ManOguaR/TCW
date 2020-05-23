// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWGameInstance.h"
#include "../TCWMobile.h"
#include "CreateSessionCallbackProxy.h"
#include "MiscFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

UTCWGameInstance::UTCWGameInstance(const FObjectInitializer& ObjectInitializer) : UGameInstance(ObjectInitializer)
{
	OnStartup.AddDynamic(this, &UTCWGameInstance::Startup);
	OnLoadMainMenu.AddDynamic(this, &UTCWGameInstance::LoadMainMenu);
	OnShowMainMenu.AddDynamic(this, &UTCWGameInstance::ShowMainMenu);
	OnShowLoadingScreen.AddDynamic(this, &UTCWGameInstance::ShowLoadingScreen);
	OnHostGameEvent.AddDynamic(this, &UTCWGameInstance::HostGame);
	OnLoadCollectionManager.AddDynamic(this, &UTCWGameInstance::LoadCollectionManager);
	OnCollectionManagerLoaded.AddDynamic(this, &UTCWGameInstance::CollectionManagerLoaded);
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

void UTCWGameInstance::LoadMainMenu()
{
	if (MoveToGameState(EGameState::GameState_MainMenu))
		UGameplayStatics::OpenLevel(this, "MainMenu");

	//DESTROY PENDING SESSIONS IF ANY
	if (SessionManager->HasActiveSession())
		SessionManager->DestroyPlayerSession(UGameplayStatics::GetPlayerController(this, 0));
}

void UTCWGameInstance::ShowMainMenu()
{
	MoveToGameState(EGameState::GameState_MainMenu);

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

	//SIGNAL STEP COMPLETED
	if (OnShowMainMenuCompleted.IsBound())
		OnShowMainMenuCompleted.Broadcast();
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

void UTCWGameInstance::LoadCollectionManager()
{
	if (MoveToGameState(EGameState::GameState_DeckBuilding))
		//../Game/Maps/DeckBuilding
		UGameplayStatics::OpenLevel(this, "CollectionManager");

	//DESTROY PENDING SESSIONS IF ANY
	if (SessionManager->HasActiveSession())
		SessionManager->DestroyPlayerSession(UGameplayStatics::GetPlayerController(this, 0));
}

void UTCWGameInstance::CollectionManagerLoaded()
{
	if (UMiscFunctionLibrary::CanExecuteCosmeticEvents(this))
	{
		if (!DeckBuilderWidgetRef->IsValidLowLevel())
		{
			FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/CollectionManager/CollectionManagerWidget.CollectionManagerWidget_C"));
			if (UClass* widgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
			{
				DeckBuilderWidgetRef = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), widgetClass);
			}
		}

		DeckBuilderWidgetRef->AddToViewport(5);
	}
}

void UTCWGameInstance::HostGame()
{
	if (OnShowLoadingScreen.IsBound())
		OnShowLoadingScreen.Broadcast();

	FTimerHandle unusedHandle;
	GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &UTCWGameInstance::HostGame_Continue, 2.0f);

}
void UTCWGameInstance::HostGame_Continue()
{
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
		// SETUP NEW 
		CurrentGameState = inState;

		//State has changed
		return true;
	}

	//Unchanged state
	return false;
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

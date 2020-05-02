// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWPlayerController.h"
#include "..\TCWMobile.h"
#include "DeckFunctionLibrary.h"
#include "GameplayFunctionLibrary.h"
#include "MiscFunctionLibrary.h"
#include "SaveGameFunctionLibrary.h"
#include "SystemFunctionLibrary.h"

#include "Kismet\DataTableFunctionLibrary.h"

#include "Blueprint\WidgetBlueprintLibrary.h"
#include "..\..\Public\Framework\TCWPlayerController.h"

ATCWPlayerController::ATCWPlayerController(const FObjectInitializer& ObjectInitializer) : APlayerController(ObjectInitializer)
{	
	//Pre-25
	//EXE-7
	OnClientPostLogin.AddDynamic(this, &ATCWPlayerController::ClientPostLoginEvent);
	OnGetPlayerDeck.AddDynamic(this, &ATCWPlayerController::GetPlayerDeckEvent);

	OnUpdateGameUI.AddDynamic(this, &ATCWPlayerController::UpdateGameUIEvent);
	OnCreateDisplayMessage.AddDynamic(this, &ATCWPlayerController::CreateDisplayMessageEvent);

	OnServerSetupDeck.AddDynamic(this, &ATCWPlayerController::ServerSetupDeckEvent);
	OnServerReturnPlayerDeck.AddDynamic(this, &ATCWPlayerController::ServerReturnPlayerDeckEvent);
	OnServerUpdateHealth.AddDynamic(this, &ATCWPlayerController::ServerUpdateHealthEvent);
	OnServerUpdatePlayerState.AddDynamic(this, &ATCWPlayerController::ServerUpdatePlayerStateEvent);

}

void ATCWPlayerController::BeginPlay()
{
	//EXE-4
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false);

	UKismetSystemLibrary::Delay(this, 2.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 2.0f);

	GameStateRef = Cast<ATCWGameState>(UGameplayStatics::GetGameState(this));

	PlayerStateRef = Cast<ATCWPlayerState>(PlayerState);
}

void ATCWPlayerController::SetupGameUI()
{
	if (IsLocalPlayerController())
	{
		//TODO: PLAYER UI WIDGET
		//WidgetBlueprint'/Game/Blueprints/Widgets/Gameplay/PlayerGameUI.PlayerGameUI'
		FStringClassReference playerUIWidgetClassRef(TEXT("/Game/Blueprints/Widgets/Gameplay/PlayerGameUI.PlayerGameUI_C"));
		if (UClass* playerUIWidgetClass = playerUIWidgetClassRef.TryLoadClass<UGameUI>())
		{
			PlayerGameUIRef = CreateWidget<UGameUI>(UGameplayStatics::GetPlayerController(this, 0), playerUIWidgetClass);
			PlayerGameUIRef->AddToViewport(2);
		}

		//TODO: OPPONENT UI WIDGET
		//WidgetBlueprint'/Game/Blueprints/Widgets/Gameplay/OpponentUI.OpponentUI'
		FStringClassReference opponentUIWidgetClassRef(TEXT("/Game/Blueprints/Widgets/Gameplay/OpponentUI.OpponentUI_C"));
		if (UClass* opponentUIWidgetClass = opponentUIWidgetClassRef.TryLoadClass<UOpponentUI>())
		{
			OpponentUIRef = CreateWidget<UOpponentUI>(UGameplayStatics::GetPlayerController(this, 0), opponentUIWidgetClass);
			OpponentUIRef->AddToViewport(1);
		}
	}
}

void ATCWPlayerController::SetTimer(int32 time)
{
	if (!CountdownTimerWidgetRef->IsValidLowLevel())
	{
		//WidgetBlueprint'/Game/Blueprints/Widgets/Extras/CountdownTimer.CountdownTimer'
		FStringClassReference MyWidgetClassRef(TEXT("/Game/Blueprints/Widgets/Extras/CountdownTimerBP.CountdownTimerBP_C"));
		if (UClass* widgetClass = MyWidgetClassRef.TryLoadClass<UCountdownTimer>())
		{
			CountdownTimerWidgetRef = CreateWidget<UCountdownTimer>(this, widgetClass);
			CountdownTimerWidgetRef->AddToViewport(5);
		}
	}
	CountdownTimerWidgetRef->Timer = time;
	if (CountdownTimerWidgetRef->Timer < 1)
		CountdownTimerWidgetRef->RemoveFromParent();

}

void ATCWPlayerController::ClientPostLoginEvent_Implementation()
{
	UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 1.0f);

	bool isSameState = false;
	USystemFunctionLibrary::GetTCWGameInstance(this)->GetGameState(EGameState::GameState_Playing, isSameState);

	if (isSameState)
	{
		SetupGameUI();
		OnUpdateGameUI.Broadcast(false);
	}
}

void ATCWPlayerController::CallCreateCardEvent(FName cardName, UDragDropOperation* operation, int32 cardHandIndex) //ECardSet cardSet
{
	if (bTurnActive)
	{
		dragDropOperationRef = operation;
		Temp_CreateCardName = cardName;
		//Temp_ChosenCardSet = cardSet;
		Temp_HandIndex = cardHandIndex;
		SetInteractionState(EPlayerState::PlayerState_Placing);
	}
}

void ATCWPlayerController::GetPlayerDeckEvent_Implementation()
{
	FString deckName;
	TArray<FName> deck = LoadClientDeck(deckName);
}

void ATCWPlayerController::UpdateGameUIEvent_Implementation(bool forceCleanUpdate)
{
	if (PlayerGameUIRef->IsValidLowLevel())
	{
		OnServerUpdateHealth.Broadcast();
		OpponentUIRef->UpdateUIPlayerStats(forceCleanUpdate);
		PlayerGameUIRef->UpdateUIPlayerStats(false);
		PlayerGameUIRef->UpdateUITurnState(bTurnActive, TurnState);
	}
}

void ATCWPlayerController::SetCountdownTimerEvent_Implementation(int32 time)
{
	SetTimer(time);
}

void ATCWPlayerController::CreateDisplayMessageEvent_Implementation(const FString& message, FLinearColor color, bool toScreen, float duration, bool toLog)
{
	CreateDisplayMessage(message, color, toScreen, duration, toLog);
}

void ATCWPlayerController::ServerSetupDeckEvent_Implementation()
{
	OnGetPlayerDeck.Broadcast();
}

void ATCWPlayerController::ServerReturnPlayerDeckEvent_Implementation(const FString& deckName, const TArray<FName>& playerDeck)
{
	SetupDeck(deckName, playerDeck);
	OnServerUpdatePlayerState.Broadcast();
}

void ATCWPlayerController::ServerUpdateHealthEvent_Implementation()
{
	if (HasAuthority())
	{
		BoardPlayerRef->OnUpdateHealth.Broadcast(Cast<ATCWPlayerState>(PlayerState)->Health);
	}
}

void ATCWPlayerController::ServerUpdatePlayerStateEvent_Implementation()
{
	if (HasAuthority())
	{
		if (GameStateRef->IsValidLowLevel())
		{
			PlayerStateRef->UpdatePlayerCardsStates(CardsInHand.Num(), PlayerDeck.Num(), GameStateRef->GetBoardState(PlayerStateRef->GamePlayerId)->PlayerCards.Num());
			OnCreateDisplayMessage.Broadcast("Deck Created!", FLinearColor(FColor::Green), false, 0.0f, true);
		}
	}
}

void ATCWPlayerController::CreateDisplayMessage(FString message, FLinearColor color, bool toScreen, float duration, bool toLog)
{
	USystemFunctionLibrary::DisplaySystemMessage(this, message, color, duration, toScreen, toLog);
}

TArray<FName> ATCWPlayerController::LoadClientDeck(FString& deckName)
{
	deckName = USystemFunctionLibrary::GetTCWGameInstance(this)->SelectedCardSet;
	bool isDeckEditable, isDeckValid = false;
	TArray<FName> resultDeck = USaveGameFunctionLibrary::LoadCustomDeck(deckName, isDeckEditable, isDeckValid);
	if (isDeckValid)
		return resultDeck;

	//DataTable'/Game/Data/Gameplay/PrebuildDeck_DataTable.PrebuildDeck_DataTable'
	TArray<FName> rowNames;
	UDataTable* prebuiltDecks;
	static ConstructorHelpers::FObjectFinder<UDataTable> dataTableObject(TEXT("DataTable'/Game/Data/Gameplay/PrebuildDeck_DataTable.PrebuildDeck_DataTable'"));
	if (dataTableObject.Succeeded())
	{
		prebuiltDecks = dataTableObject.Object;
		UDataTableFunctionLibrary::GetDataTableRowNames(prebuiltDecks, rowNames);
	}

	resultDeck = USaveGameFunctionLibrary::LoadCustomDeck(rowNames[FMath::RandRange(0, rowNames.Num() - 1)].ToString(), isDeckEditable, isDeckValid);
	if (isDeckValid)
		return resultDeck;

	OnCreateDisplayMessage.Broadcast(TEXT("Unable to load a valid deck"), FLinearColor(FColor::Red), true, 0.0f, true);
	deckName = TEXT("INVALID");
	return TArray<FName>();
}

ATCWPawn* ATCWPlayerController::CreatePlaceableCard_Client(FName cardName, FVector location)
{
	//TODO: ATCWPlayerController::CreatePlaceableCard_Client
	// bool cameraValid;
	//GetWorld()->SpawnActor<ATCWPawn>(location, UGameplayFunctionLibrary::GetWorldRotationForPlayer(this, FRotator(0.0f), cameraValid));

	//Blueprint'/Game/Blueprints/Gameplay/BoardPawn.BoardPawn'



	//UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/DEXIED/Foliage/Tree/BP_TreeDestroyed_Style_1.BP_TreeDestroyed_Style_1")));

	//UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	//if (!SpawnActor)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
	//	return nullptr;
	//}

	//UClass* SpawnClass = SpawnActor->StaticClass();
	//if (SpawnClass == NULL)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
	//	return nullptr;
	//}

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	//GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, location, UGameplayFunctionLibrary::GetWorldRotationForPlayer(this, FRotator(0.0f), cameraValid), SpawnParams);





	return nullptr;
}

ATCWPawn* ATCWPlayerController::CreatePlaceableCard_Server(FTransform SpawnTransform)
{
	//TODO: ATCWPlayerController::CreatePlaceableCard_Server
	return nullptr;
}

void ATCWPlayerController::SetupDeck(FString deckName, TArray<FName> playerDeck)
{
	if (HasAuthority())
	{
		//UE_LOG(TCWLog, Log, TEXT(&FString::Printf("Player: {0} Chosen Deck: {1}", UKismetSystemLibrary::GetDisplayName(this), deckName)));

		TempDeck = playerDeck;

		if (bShuffleDeck)
			ShufflePlayerDeck(TempDeck);

		if (bEnableWeightedCards)
			PlayerDeck = UDeckFunctionLibrary::FilterWeightedCardsInDeck(TempDeck);
		else
			PlayerDeck = TempDeck;

		Cast<ATCWPlayerState>(PlayerState)->MaxCardsInDeck = PlayerDeck.Num();
	}
}

void ATCWPlayerController::ShufflePlayerDeck(TArray<FName>& playerDeck)
{
	if (playerDeck.Num() > 0)
	{
		int32 LastIndex = playerDeck.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				playerDeck.Swap(i, Index);
			}
		}
	}
}

void ATCWPlayerController::SetInteractionState(EPlayerState changeToState)
{
	if (changeToState != PlayerStateEnum)
		PlayerStateEnum = changeToState;
}

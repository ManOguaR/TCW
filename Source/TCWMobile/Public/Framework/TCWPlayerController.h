// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BoardPlayer.h"
#include "CountdownTimer.h"
#include "Enums.h"
#include "GameUI.h"
#include "OpponentUI.h"
#include "TCWGameState.h"
#include "TCWPawn.h"
#include "TCWPlayerState.h"

#include "TCWPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWPlayerControllerSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateGameUISignature, bool, forceCleanUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountdownTimerSignature, int32, time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReturnPlayerDeckSignature, const FString&, deckName, const TArray<FName>&, playerDeck);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCreateCardSignature, FName, cardName, UDragDropOperation*, operation, int32, cardHandIndex); //ECardSet cardSet
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FControllerMessageSignature, const FString&, message, FLinearColor, color, bool, toScreen, float, duration, bool, toLog);

/**
 *
 */
UCLASS()
class TCWMOBILE_API ATCWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATCWPlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	//InputTouch();
	//LeftMouseButton();

public:
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerSignature OnClientPostLogin;
	UPROPERTY(BlueprintCallable)
		FCreateCardSignature OnCallCreateCard;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerSignature OnGetPlayerDeck;

	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FUpdateGameUISignature OnUpdateGameUI;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FCountdownTimerSignature OnSetCountdownTimer;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FControllerMessageSignature OnCreateDisplayMessage;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerSignature OnServerSetupDeck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FReturnPlayerDeckSignature OnServerReturnPlayerDeck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerSignature OnServerUpdateHealth;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerSignature OnServerUpdatePlayerState;

public:
	UPROPERTY(EditAnywhere, Category = "Game Deck")
		bool bShuffleDeck;
	UPROPERTY(EditAnywhere, Category = "Game Deck")
		bool bEnableWeightedCards;

	UPROPERTY(EditAnywhere, Category = "Game Deck")
		int32 MaxCardsInHand;
	UPROPERTY(EditAnywhere, Category = "Game Deck")
		int32 CardsInFirstHand;
	UPROPERTY(EditAnywhere, Category = "Game Deck")
		int32 CardsToDrawPerTurn;

	UPROPERTY(EditAnywhere, Category = "Player")
		ABoardPlayer* BoardPlayerRef;

private:
	//Game Deck related variables
	TArray<FName> TempDeck;
	TArray<FName> PlayerDeck;
	int32 weightedFilterIndex;
	UCountdownTimer* CountdownTimerWidgetRef;

	//Card Manager related variables
	//TODO: Replication
	TArray<FName> CardsInHand;
	//Temp related variables
	FName Temp_CreateCardName;
	//ECardSet Temp_ChosenCardSet;
	int32 Temp_HandIndex;

	//System related variables
	ATCWGameState* GameStateRef;
	ATCWPlayerState* PlayerStateRef;
	EGameTurn TurnState;

	//Player related variables
	EPlayerState PlayerStateEnum;
	bool bTurnActive;
	UGameUI* PlayerGameUIRef;
	UOpponentUI* OpponentUIRef;

	//Player related variables
	UDragDropOperation* dragDropOperationRef;

public:
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void SetupGameUI();
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void SetTimer(int32 time);

private:
	UFUNCTION(Client, Reliable)
		void ClientPostLoginEvent();
	UFUNCTION()
		void CallCreateCardEvent(FName cardName, UDragDropOperation* operation, int32 cardHandIndex); //ECardSet cardSet
	UFUNCTION(Client, Reliable)
		void GetPlayerDeckEvent();

	UFUNCTION(Client, Reliable)
		void UpdateGameUIEvent(bool forceCleanUpdate);
	UFUNCTION(Client, Reliable)
		void SetCountdownTimerEvent(int32 time);
	UFUNCTION(Client, Unreliable)
		void CreateDisplayMessageEvent(const FString& message, FLinearColor color, bool toScreen, float duration, bool toLog);

	UFUNCTION(Server, Unreliable)
		void ServerSetupDeckEvent();
	UFUNCTION(Server, Reliable)
		void ServerReturnPlayerDeckEvent(const FString& deckName, const TArray<FName>& playerDeck);
	UFUNCTION(Server, Reliable)
		void ServerUpdateHealthEvent();
	UFUNCTION(Server, Reliable)
		void ServerUpdatePlayerStateEvent();

private:
	void CreateDisplayMessage(FString message, FLinearColor color, bool toScreen, float duration, bool toLog);
	//game functions
	//card functions
	ATCWPawn* CreatePlaceableCard_Client(FName cardName, FVector location); //ECardSet cardSet
	ATCWPawn* CreatePlaceableCard_Server(FTransform SpawnTransform);
	//card interaction functions
	//game setup functions
	void SetupDeck(FString deckName, TArray<FName> playerDeck);
	TArray<FName> LoadClientDeck(FString& deckName);
	//deck/hand functions
	void ShufflePlayerDeck(TArray<FName>& playerDeck);

	void SetInteractionState(EPlayerState changeToState);
};

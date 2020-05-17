// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "BoardPlayer.h"
#include "CardsInHandInterface.h"
#include "ControllerInterface.h"
#include "CountdownTimer.h"
#include "DeckInterface.h"
#include "Enums.h"
#include "GameStateInterface.h"
#include "GameUI.h"
#include "OpponentUI.h"
#include "TCWGameState.h"
#include "TCWPawn.h"
#include "TCWPlayerState.h"

#include "TCWPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWPlayerControllerEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTCWPlayerControllerBoolEvent, bool, value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCardEvent, FName, card);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountdownTimerEvent, int32, time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReturnPlayerDeckEvent, const FString&, deckName, const TArray<FName>&, playerDeck);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCreateCardEvent, FName, cardName, UDragDropOperation*, operation, int32, cardHandIndex, ECardSet, cardSet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FControllerMessageEvent, const FString&, message, FLinearColor, color, bool, toScreen, float, duration, bool, toLog);

/**
 *
 */
UCLASS()
class TCWMOBILE_API ATCWPlayerController : public APlayerController, public IControllerInterface, public IDeckInterface, public IGameStateInterface, public ICardsInHandInterface
{
	GENERATED_BODY()

public:
	ATCWPlayerController(const FObjectInitializer& ObjectInitializer);

	//void BeginPlay() final;
	//InputTouch();
	//LeftMouseButton();

public:
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerEvent OnClientPostLogin;
	UPROPERTY(BlueprintCallable)
		FCreateCardEvent OnCallCreateCard;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerEvent OnGetPlayerDeck;

	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerEvent OnDragCancelled;

	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FTCWPlayerControllerBoolEvent OnUpdateGameUI;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FCountdownTimerEvent OnSetCountdownTimer;
	UPROPERTY(BlueprintCallable, Category = "Client Events")
		FControllerMessageEvent OnCreateDisplayMessage;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnServerSetupDeck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnServerRequestChangeTurnState;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FReturnPlayerDeckEvent OnServerReturnPlayerDeck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnServerUpdateHealth;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnServerUpdatePlayerState;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerBoolEvent OnSetSkipManaCheck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnReshuffleDeck;
	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FTCWPlayerControllerEvent OnClearCardsInHand;

	UPROPERTY(BlueprintCallable, Category = "Server Events")
		FCardEvent OnDeveloper_AddCardToHand;
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

	UPROPERTY(BlueprintReadWrite, Category = "Player")
		ABoardPlayer* BoardPlayerRef;

	//Player related variables
	UGameUI* PlayerGameUIRef;
	bool bTurnActive;

private:
	UCountdownTimer* CountdownTimerWidgetRef;

	//Game Deck related variables
	TArray<FName> TempDeck;
	TArray<FName> PlayerDeck;
	int32 weightedFilterIndex;

	//Card Manager related variables
	//TODO: Replication
	TArray<FName> CardsInHand;
	FName CardToAdd;

	//Temp related variables
	FName Temp_CreateCardName;
	ECardSet Temp_ChosenCardSet;
	int32 Temp_HandIndex;

	//System related variables
	ATCWGameState* GameStateRef;
	ATCWPlayerState* PlayerStateRef;
	EGameTurn TurnState;

	//Player related variables
	EPlayerState PlayerStateEnum;
	UOpponentUI* OpponentUIRef;

	//Player related variables
	UDragDropOperation* dragDropOperationRef;

	//Developer
	bool bSkipManaCheck;

public:
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void SetupGameUI();
	UFUNCTION(BlueprintCallable, Category = "Game Setup")
		void SetTimer(int32 time);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		ABoardUnit* CreatePlayableCard(FTransform spawnTransform);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool AddCardToPlayersHand(FName cardName);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginPlayerTurn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndPlayerTurn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		TArray<FName> GetPlayerDeck();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RemoveCardFromDeck(bool removeAll, int32 indexToRemove);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void MatchEnd(EEndGameResults endGameResult);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void MatchBegin();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChangeActivePlayerTurn(bool turnActive);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RequestChangePlayerTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RemoveCardFromHand(FName cardName, int32 index, bool removeAll);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DrawCard(FName cardName, bool ignoreMaxCards, int32 numberOfCardsToDraw);

private:
	UFUNCTION(Client, Reliable)
		void ClientPostLogin();

	UFUNCTION()
		void CallCreateCard(FName cardName, UDragDropOperation* operation, int32 cardHandIndex, ECardSet cardSet);
	UFUNCTION(Client, Reliable)
		void GetPlayerDeckEvent();

	UFUNCTION()
		void DragCancelled();

	UFUNCTION(Client, Reliable)
		void UpdateGameUI(bool value);
	UFUNCTION(Client, Reliable)
		void SetCountdownTimer(int32 time);
	UFUNCTION(Client, Unreliable)
		void CreateDisplayMessage(const FString& message, FLinearColor color, bool toScreen, float duration, bool toLog);

	UFUNCTION(Server, Unreliable)
		void ServerSetupDeck();
	UFUNCTION(Server, Reliable)
		void ServerRequestChangeTurnState();
	UFUNCTION(Server, Reliable)
		void ServerReturnPlayerDeck(const FString& deckName, const TArray<FName>& playerDeck);
	UFUNCTION(Server, Reliable)
		void ServerUpdateHealth();
	UFUNCTION(Server, Reliable)
		void ServerUpdatePlayerState();
	UFUNCTION(Server, Unreliable)
		void SetSkipManaCheck(bool value);
	UFUNCTION(Server, Unreliable)
		void ReshuffleDeck();
	UFUNCTION(Server, Unreliable)
		void ClearCardsInHand();
	UFUNCTION(Server, Unreliable)
		void Developer_AddCardToHand(FName cardToAdd);

private:
	void CreateDisplayMessageInternal(FString message, FLinearColor color, bool toScreen, float duration, bool toLog);
	//game functions
	//card functions
	ATCWPawn* CreatePlaceableCard_Client(FName cardName, FVector location, ECardSet cardSet); //ECardSet cardSet
	ATCWPawn* CreatePlaceableCard_Server(FTransform SpawnTransform);
	//card interaction functions
	//game setup functions
	void SetupDeck(FString deckName, TArray<FName> playerDeck);
	TArray<FName> LoadClientDeck(FString& deckName);
	//deck/hand functions
	void ShufflePlayerDeck(TArray<FName>& playerDeck);

	void SetInteractionState(EPlayerState changeToState);

	void AddCardToHandInternal();

protected:
	void BeginPlay() final;

private:
	void BeginPlay_Delayed();
	void ClientPostLogin_Continue();
};

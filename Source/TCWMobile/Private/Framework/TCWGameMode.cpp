// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWGameMode.h"
#include "../TCWMobile.h"
#include "TCWGameMode.h"
#include "TCWGameState.h"
#include "TCWHUD.h"
#include "TCWPawn.h"
#include "TCWPlayerController.h"
#include "TCWPlayerState.h"
#include "TCWSpectator.h"

#include "BoardPlayer.h"
#include "CotrollerFunctionLibrary.h"
#include "MiscFunctionLibrary.h"
#include "SystemFunctionLibrary.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ATCWGameMode::ATCWGameMode(const FObjectInitializer& ObjectInitializer) : AGameMode(ObjectInitializer)
{
	//Pre-21
	//EXE-5
	DefaultPawnClass = ATCWPawn::StaticClass();
	HUDClass = ATCWHUD::StaticClass();
	PlayerControllerClass = ATCWPlayerController::StaticClass();
	GameStateClass = ATCWGameState::StaticClass();
	PlayerStateClass = ATCWPlayerState::StaticClass();
	SpectatorClass = ATCWSpectator::StaticClass();

	OnCheckGamePreconditions.AddDynamic(this, &ATCWGameMode::CheckGamePreconditionsEvent);
	OnForceSpawnAIOpponent.AddDynamic(this, &ATCWGameMode::ForceSpawnAIOpponentEvent);
	OnGameStartCountdown.BindUFunction(this, FName("GameStartCountdownEvent"));
	OnEndGame.AddDynamic(this, &ATCWGameMode::EndGameEvent);
}

void ATCWGameMode::BeginPlay()
{
	//EXE-2-2
	UKismetSystemLibrary::Delay(this, 0.2f, FLatentActionInfo());
	//UMiscFunctionLibrary::Delay(this, 0.2f);

	GameStateRef = Cast<ATCWGameState>(UGameplayStatics::GetGameState(this));
}

void ATCWGameMode::BeginPlayDelayed()
{

}

void ATCWGameMode::PostLogin(APlayerController* NewPlayer)
{
	//EXE-2-1
	if (HasAuthority())
	{
		if (UGameplayStatics::GetCurrentLevelName(this) != "MainMenu")
		{
			if ((GameControllersArray.Num() < MaxNumOfPlayers) && (!bSpectator))
			{
				AController* playerController = AddPlayerToArray(NewPlayer->PlayerState, NewPlayer);
				SetGamePlayerId(playerController);
				SetBoardPlayerReference();
				SetBoardPlayerReferences(playerController);

				ATCWPlayerState* state = nullptr;
				bool isValid = false;
				ATCWPlayerController* controllerRef = UCotrollerFunctionLibrary::GetTCWPlayerControllerReference(playerController, state, isValid);
				controllerRef->OnClientPostLogin.Broadcast();
				controllerRef->OnServerSetupDeck.Broadcast();
			}
			else
			{
				SetBoardPlayerReference();
			}

			OnCheckGamePreconditions.Broadcast();
		}
	}
}

void ATCWGameMode::Logout(AController* Exiting)
{
	RemovePlayerFromGame(Exiting);
}

AController* ATCWGameMode::AddPlayerToArray(AActor* playerState, AController* playerController)
{
	PlayerStateArray.AddUnique(playerState);
	GameControllersArray.AddUnique(playerController);

	if (GameStateRef->IsValidLowLevel())
	{
		GameStateRef->PlayerStateArray = PlayerStateArray;
	}
	return playerController;
}

void ATCWGameMode::GetPlayerControllers(AController* player1, AController* player2)
{
	player1 = GameControllersArray[0];
	player2 = (GameControllersArray.Num() > 1 ? GameControllersArray[1] : nullptr);
}

void ATCWGameMode::SetGamePlayerId(AController* playerController)
{
	ATCWPlayerState* state = nullptr;
	bool isValid = false;
	ATCWPlayerController* controllerRef = UCotrollerFunctionLibrary::GetTCWPlayerControllerReference(playerController, state, isValid);

	state->GamePlayerId = GameControllersArray.Num();
	state->OwningPlayerController = controllerRef;
}

void ATCWGameMode::SetBoardPlayerReferences(AController* controller)
{
	ATCWPlayerState* state = nullptr;
	bool isValid = false;
	ATCWPlayerController* controllerRef = UCotrollerFunctionLibrary::GetTCWPlayerControllerReference(controller, state, isValid);

	if (isValid)
	{
		ABoardPlayer* bp = BoardPlayersArray[GameControllersArray.Find(controller)];
		controllerRef->BoardPlayerRef = bp;
		bp->OwningController = controllerRef;
	}
	else
	{
		AAIGamePawn* aiState = nullptr;
		AAIPlayerController* aiControllerRef = UCotrollerFunctionLibrary::GetAiControllerReference(controller, aiState, isValid);

		if (isValid)
		{
			ABoardPlayer* bp = BoardPlayersArray[GameControllersArray.Find(controller)];
			aiControllerRef->BoardPlayerRef = bp;
			bp->OwningController = aiControllerRef;
		}
	}
}

void ATCWGameMode::RemovePlayerFromGame(AController* controller)
{
	GameControllersArray.Remove(controller);
}

void ATCWGameMode::CreateCardGameAIOpponent()
{
	if (GameControllersArray.Num() < MaxNumOfPlayers)
	{
		if (bSpectator)
		{
			for (int a = 1; a < 3; a++)
			{
				CreateandAddAIOpponent();
			}
		}
		else
		{
			CreateandAddAIOpponent();
		}
	}
}

int32 ATCWGameMode::CalculateManaForTurn(int32 turn)
{
	return FMath::Clamp(turn + Mana_Min, Mana_Min, Mana_Max);
}

int32 ATCWGameMode::GetTurnMana(AController* controller)
{
	int32 controllerId = UCotrollerFunctionLibrary::GetControllerId(controller);

	int32 health, numCardsInHand, cardsInDeck, activeCards, mana, manaMax, playerTurn;
	TArray<FName> deck, cardsInHand;
	AActor* playerState = nullptr;
	UCotrollerFunctionLibrary::GetControllersStateProfile(controllerId, health, numCardsInHand, cardsInDeck, activeCards, mana, manaMax, playerTurn, deck, cardsInHand, playerState);

	return CalculateManaForTurn(playerTurn);
}

void ATCWGameMode::SetBoardPlayerReference()
{
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(this, ABoardPlayer::StaticClass(), outActors);

	for (AActor* act : outActors)
	{
		if (ABoardPlayer* bp = Cast<ABoardPlayer>(act))
			BoardPlayersArray[bp->PlayerIndex - 1] = bp;
	}
}

void ATCWGameMode::CreateandAddAIOpponent()
{
	APawn* aiState = UAIBlueprintHelperLibrary::SpawnAIFromClass(this, AAIGamePawn::StaticClass(), nullptr, BoardPlayersArray[GameControllersArray.Num()]->GetActorLocation());
	AController* aiController = aiState->GetController();
	AddPlayerToArray(aiState, aiController);
	SetBoardPlayerReferences(aiController);
	AAIGamePawn* aiPawn = Cast<AAIGamePawn>(aiState);
	aiPawn->GamePlayerId = GameControllersArray.Num();
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAIGamePawn::StaticClass(), outActors);
	aiPawn->GameAiId = outActors.Num();
}

bool ATCWGameMode::CheckIsPlayerActiveState(int32 controllerId)
{
	int32 health, numCardsInHand, cardsInDeck, activeCards, mana, manaMax, playerTurn;
	TArray<FName> deck, cardsInHand;
	AActor* playerState = nullptr;
	UCotrollerFunctionLibrary::GetControllersStateProfile(controllerId, health, numCardsInHand, cardsInDeck, activeCards, mana, manaMax, playerTurn, deck, cardsInHand, playerState);

	return (health <= 0 ? false : (numCardsInHand > 0 || cardsInDeck > 0 || activeCards > 0));
}

void ATCWGameMode::CheckGamePreconditionsEvent_Implementation()
{
	if (HasAuthority())
	{
		if (GameControllersArray.Num() != MaxNumOfPlayers)
		{
			if (!bSingleClientEnabled)
			{
				UE_LOG(TCWLog, Log, TEXT("Waiting on one more player..."));
			}
			else
			{
				UE_LOG(TCWLog, Log, TEXT("Standalone Client ENABLED"));
				CreateCardGameAIOpponent();
			}
		}
		UE_LOG(TCWLog, Log, TEXT("All players are in!"));
		if (bSkipStartTimer)
		{
			UKismetSystemLibrary::Delay(this, 3.0f, FLatentActionInfo());
			//UMiscFunctionLibrary::Delay(this, 3.0f);
			bGameActive = true;
			GameStateRef->OnGameStart.Broadcast();
		}
		else
		{
			timerHandle = UKismetSystemLibrary::K2_SetTimerDelegate(OnGameStartCountdown, gameSeconds, true);
		}
	}
}

void ATCWGameMode::ForceSpawnAIOpponentEvent_Implementation()
{
	CreateCardGameAIOpponent();
	UE_LOG(TCWLog, Log, TEXT("All players are in!"));
	if (bSkipStartTimer)
	{
		UKismetSystemLibrary::Delay(this, 3.0f, FLatentActionInfo());
		//UMiscFunctionLibrary::Delay(this, 3.0f);
		bGameActive = true;
		GameStateRef->OnGameStart.Broadcast();
	}
	else
	{
		timerHandle = UKismetSystemLibrary::K2_SetTimerDelegate(OnGameStartCountdown, gameSeconds, true);
	}
}

void ATCWGameMode::GameStartCountdownEvent_Implementation()
{
	countdownTimer = countdownTimer - 1;
	for (AController* cont : GameControllersArray)
	{
		if (ATCWPlayerController* pController = Cast<ATCWPlayerController>(cont))
		{
			pController->OnSetCountdownTimer.Broadcast(countdownTimer);
		}
	}
	if (countdownTimer < 1)
	{
		UKismetSystemLibrary::K2_ClearTimerHandle(this, timerHandle);

		bGameActive = true;
		GameStateRef->OnGameStart.Broadcast();
	}
}

void ATCWGameMode::EndGameEvent_Implementation()
{
	if (HasAuthority())
	{
		bGameActive = false;

		bool p1Active = CheckIsPlayerActiveState(1);
		bool p2Active = CheckIsPlayerActiveState(2);

		EEndGameResults	p1Result = (p1Active == p2Active ? EEndGameResults::GameResult_Draw : (p1Active ? EEndGameResults::GameResult_Victory : EEndGameResults::GameResult_Defeat));
		EEndGameResults	p2Result = (p2Active == p1Active ? EEndGameResults::GameResult_Draw : (p2Active ? EEndGameResults::GameResult_Victory : EEndGameResults::GameResult_Defeat));

		GameStateRef->OnNotifyEndGameState.Broadcast(p1Result, p2Result);
	}
}

void ATCWGameMode::CheckPlayerStateEvent_Implementation()
{
	if (!(CheckIsPlayerActiveState(1) && CheckIsPlayerActiveState(2)))
	{
		OnEndGame.Broadcast();
	}
}

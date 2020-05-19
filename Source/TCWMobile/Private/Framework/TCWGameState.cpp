// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWGameState.h"
#include "..\TCWMobile.h"
#include "ControllerFunctionLibrary.h"
#include "MiscFunctionLibrary.h"
#include "SystemFunctionLibrary.h"
#include "TCWGameMode.h"

#include "Kismet\GameplayStatics.h"

ATCWGameState::ATCWGameState(const FObjectInitializer& ObjectInitializer) : AGameState(ObjectInitializer)
{
	OnGameStart.AddDynamic(this, &ATCWGameState::GameStart);
	OnNotifyEndGameState.AddDynamic(this, &ATCWGameState::NotifyEndGameState);
	OnServerRequestChangeTurnState.AddDynamic(this, &ATCWGameState::ServerRequestChangeTurnState);
}

void ATCWGameState::BeginPlay()
{
	//EXE-3
	if (HasAuthority())
	{
		FTimerHandle unusedHandle;
		GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &ATCWGameState::BeginPlay_Delayed, 0.5f);
	}
}
void ATCWGameState::BeginPlay_Delayed()
{
	GameModeRef = Cast<ATCWGameMode>(UGameplayStatics::GetGameMode(this));
	CompilePlacementsPerPlayer();
	GetGraveyardReferencePerPlayer();
}

UBoardState* ATCWGameState::GetBoardState(int32 playerID)
{
	return nullptr;
}

bool ATCWGameState::RequestChangeTurnState(AController* controller)
{
	return controller == PlayerTurnArray[0];
}

void ATCWGameState::GameStart_Implementation()
{
	if (HasAuthority())
	{
		bGameActive = true;
		PlayerStateArray = GameModeRef->GetPlayerStateArray();
	}
}

void ATCWGameState::NotifyEndGameState_Implementation(EEndGameResults player1, EEndGameResults player2)
{
	if (HasAuthority())
	{
		bGameActive = false;
		UKismetSystemLibrary::K2_ClearTimerHandle(this, turnTimerRef);
		UKismetSystemLibrary::K2_ClearTimerHandle(this, gameTimerRef);

		TArray<AController*> controllers = GameModeRef->GetGameControllersArray();
		for (int a = 0; a < controllers.Num(); a++)
		{
			//TODO. GAME STATEINTERFACE MATCH END
			//controllers[a]->MatchEnd(a == 0 ? player1 : player2);
		}

		FTimerHandle unusedHandle;
		GetWorld()->GetTimerManager().SetTimer(unusedHandle, this, &ATCWGameState::NotifyEndGameState_Continue, 0.5f);
	}
}

void ATCWGameState::NotifyEndGameState_Continue()
{
	USystemFunctionLibrary::GetTCWGameInstance(this)->OnShowMainMenu.Broadcast();
}

void ATCWGameState::ServerRequestChangeTurnState_Implementation(AController* controller)
{
	if (RequestChangeTurnState(controller))
	{
		ChangeTurnState();
	}
}

void ATCWGameState::ChangeTurnState()
{
	//TODO:
	if (bGameActive)
	{
		RotatePlayerTurn();
		SetPlayerTurnsActive();
	}
}

void ATCWGameState::RotatePlayerTurn()
{
	PlayerTurnArray.Add(PlayerTurnArray[0]);
	PlayerTurnArray.RemoveAt(0);
}

void ATCWGameState::SetPlayerTurnsActive()
{
	for (AController* controller : GameModeRef->GetGameControllersArray())
	{
		if (IGameStateInterface* contInterface = Cast<IGameStateInterface>(controller))
		{
			contInterface->ChangeActivePlayerTurn(PlayerTurnArray[0] == controller);
		}

		if (PlayerTurnArray[0] == controller)
		{
			BeginPlayerTurn(UControllerFunctionLibrary::GetControllerId(controller));
		}
		else
		{
			EndPlayerTurn(UControllerFunctionLibrary::GetControllerId(controller));
		}
	}

	ResetTurnTimer();
}

void ATCWGameState::BeginPlayerTurn(int32 playerId)
{
	if (HasAuthority())
	{
		TArray<ABoardUnit*> playerUnits = GetBoardState(playerId)->PlayerUnits;

		for (ABoardUnit* each : playerUnits)
		{
			each->OnActivePlayerTurn();
		}
	}
}

void ATCWGameState::EndPlayerTurn(int32 playerId)
{
	if (HasAuthority())
	{
		TArray<ABoardUnit*> playerUnits = GetBoardState(playerId)->PlayerUnits;

		for (ABoardUnit* each : playerUnits)
		{
			each->OnEndActivePlayerTurn();
		}
	}
}

void ATCWGameState::ResetTurnTimer()
{
	TurnTimeSeconds = turnDurationSeconds;
	TurnTimeMinutes = turnDurationMinutes;
}

void ATCWGameState::CompilePlacementsPerPlayer()
{
	//TODO: ATCWGameState::CompilePlacementsPerPlayer
}

void ATCWGameState::GetGraveyardReferencePerPlayer()
{

}

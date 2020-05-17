// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "BoardState.h"
#include "SystemEnums.h"

#include "TCWGameState.generated.h"

class ATCWGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWGameStateEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeTurnStateEvent, AController*, controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyEndGameEvent, EEndGameResults, player1, EEndGameResults, player2);

/**
 *
 */
UCLASS()
class TCWMOBILE_API ATCWGameState : public AGameState
{
	GENERATED_BODY()

public:
	ATCWGameState(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void BeginPlay_Delayed();

public:
	//TODO: Replicated
	TArray<AActor*> PlayerStateArray;

private:
	ATCWGameMode* GameModeRef;
	UBoardState* BoardStateRef;
	bool bGameActive;

	TArray<AController*> PlayerTurnArray;

	FTimerHandle turnTimerRef;
	FTimerHandle gameTimerRef;
public:
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameStateEvent OnGameStart;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FNotifyEndGameEvent OnNotifyEndGameState;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FChangeTurnStateEvent OnServerRequestChangeTurnState;

	UBoardState* GetBoardState(int32 playerID);

	bool RequestChangeTurnState(AController* controller);

private:
	UFUNCTION(Server, Unreliable)
		void GameStart();

	UFUNCTION(Server, Reliable)
		void NotifyEndGameState(EEndGameResults player1, EEndGameResults player2);
	void NotifyEndGameState_Continue();

	UFUNCTION(Server, Reliable)
		void ServerRequestChangeTurnState(AController* controller);

	void ChangeTurnState();

	void RotatePlayerTurn();
	void SetPlayerTurnsActive();
};

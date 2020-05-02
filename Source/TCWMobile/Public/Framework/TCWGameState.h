// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"

#include "BoardState.h"
#include "SystemEnums.h"

#include "TCWGameState.generated.h"

class ATCWGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWGameStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNotifyEndGameSignature, EEndGameResults, player1, EEndGameResults, player2);

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

public:
	//TODO: Replicated
	TArray<AActor*> PlayerStateArray;

private:
	ATCWGameMode* GameModeRef;
	UBoardState* BoardStateRef;
	bool bGameActive;

	FTimerHandle turnTimerRef;
	FTimerHandle gameTimerRef;
public:
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameStateSignature OnGameStart;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FNotifyEndGameSignature OnNotifyEndGameState;

	UBoardState* GetBoardState(int32 playerID);

private:
	UFUNCTION(Server, Unreliable)
		void GameStartEvent();
	UFUNCTION(Server, Reliable)
		void NotifyEndGameStateEvent(EEndGameResults player1, EEndGameResults player2);
};

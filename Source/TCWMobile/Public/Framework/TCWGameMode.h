// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "TCWGameMode.generated.h"

class ATCWGameState;
class ABoardPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTCWGameModeEvent);

/**
 *
 */
UCLASS()
class TCWMOBILE_API ATCWGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATCWGameMode(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void BeginPlayDelayed();

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, Category = "System")
		void GetPlayerControllers(AController* player1, AController* player2);
	UFUNCTION(BlueprintCallable, Category = "System")
		void SetGamePlayerId(AController* controller);
	UFUNCTION(BlueprintCallable, Category = "System")
		AController* AddPlayerToArray(AActor* playerState, AController* playerController);
	UFUNCTION(BlueprintCallable, Category = "System")
		void SetBoardPlayerReferences(AController* controller);
	UFUNCTION(BlueprintCallable, Category = "System")
		void RemovePlayerFromGame(AController* controller);

	UFUNCTION(BlueprintCallable, Category = "AI Opponent")
		void CreateCardGameAIOpponent();

	UFUNCTION(BlueprintPure, Category = "System")
		int32 CalculateManaForTurn(int32 turn);
	UFUNCTION(BlueprintPure, Category = "System")
		int32 GetTurnMana(AController* controller);
	UFUNCTION(BlueprintCallable, Category = "System")
		void SetBoardPlayerReference();

	TArray<AActor*> GetPlayerStateArray() const { return PlayerStateArray; };
	TArray<AController*> GetGameControllersArray() const { return GameControllersArray; };

public:
	bool bSkipStartTimer;

private:
	float gameSeconds;
	int32 countdownTimer;
	ATCWGameState* GameStateRef;
	bool bGameActive;
	TArray<AController*> GameControllersArray;
	TArray<AActor*> PlayerStateArray;
	TArray<ABoardPlayer*> BoardPlayersArray;

	FTimerHandle timerHandle;

public:
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameModeEvent OnCheckGamePreconditions;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameModeEvent OnForceSpawnAIOpponent;
	UPROPERTY()
		FTimerDynamicDelegate OnGameStartCountdown;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameModeEvent OnEndGame;
	UPROPERTY(BlueprintCallable, Category = "Game Mode Events")
		FTCWGameModeEvent OnCheckPlayerState;

private:
	UFUNCTION(Server, Reliable)
		void CheckGamePreconditions();
	UFUNCTION(Server, Unreliable)
		void ForceSpawnAIOpponent();
	UFUNCTION(Server, Unreliable)
		void GameStartCountdown();
	UFUNCTION(Server, Reliable)
		void EndGame();
	UFUNCTION(Server, Reliable)
		void CheckPlayerState();

	void CreateandAddAIOpponent();
	bool CheckIsPlayerActiveState(int32 controllerId);

public:
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		int32 Mana_Min;
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		int32 Mana_Max;
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		int32 PlayerStartingHealth;
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		int32 MaxNumOfPlayers;
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		bool bSingleClientEnabled;

private:
	UPROPERTY(EditAnywhere, Category = "Game Mode Settings")
		bool bSpectator;
};

// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Enums.h"
#include "PlayerUIInterface.h"

#include "GameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameUIEvent);

class ATCWPlayerState;
class ATCWGameState;
class UCardManager;
class UMessageManager;
class UGameProfileUI;

class UButton;
class UCanvasPanel;
class UTextBlock;
class UVerticalBox;
class UWidgetAnimation;

/**
 *
 */
UCLASS()
class TCWMOBILE_API UGameUI : public UUserWidget, public IPlayerUIInterface
{
	GENERATED_BODY()

public:
	UGameUI(const FObjectInitializer& ObjectInitializer);

	UCardManager* GetCardManager();

	UPROPERTY(BlueprintCallable, Category = "Events")
		FGameUIEvent OnToggleGameUI;
	UPROPERTY()
		FTimerDynamicDelegate OnUpdateGameAndTurnTimers;
	UPROPERTY()
		FTimerDynamicDelegate OnCheckTurnBeginState;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* ChangeTurn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
		UWidgetAnimation* SpawnAIOpponent;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCardManager* CardManager;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* PlayerUI_Canvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UGameProfileUI* PlayerProfileUI;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UMessageManager* MessageManager;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* SpawnAIOpponent_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ToggleMessageManager_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* TurnButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TurnButton_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* GameTime;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* WaitingOnPlayer_VerticalBox;

private:
	bool bIsVisible;
	bool bIsMessageManagerVisible;
	ATCWPlayerState* PlayerStateRef;
	ATCWGameState* GameStateRef;
	bool bPlayerTurnActive;
	int32 gameSeconds;
	int32 gameMinutes;
	int32 turnSeconds;
	int32 turnMinutes;

	FTimerHandle updateGameTimerHandle;
	FTimerHandle checkTurnTimerHandle;

	EGameTurn currentTurnState;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUIPlayerStats(bool forceCleanVisualUpdate);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUITurnState(bool turnActive, EGameTurn turnState);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//	void UpdateUIGameTurnTime();
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//	void UpdatePlayerStateUI(int32 p1, int32 p2, int32 p3, int32 p4);

private:
	UFUNCTION()
		void ToggleGameUI();
	UFUNCTION()
		void OnSpawnAIOpponent_ButtonClicked();
	UFUNCTION()
		void OnToggleMessageManager_ButtonClicked();
	UFUNCTION()
		void OnTurnButtonClicked();
	UFUNCTION()
		void UpdateGameAndTurnTimers();
	UFUNCTION()
		void CheckTurnBeginState();

protected:
	//virtual void NativeOnInitialized();
	void NativePreConstruct() override;
	void NativeConstruct() override;
	//virtual void NativeDestruct();

private:
	FString CombineTimer(int32 seconds, int32 minutes);
};

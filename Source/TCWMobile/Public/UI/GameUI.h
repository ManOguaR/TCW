// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

#include "Enums.h"
#include "MessageManager.h"
#include "PlayerUIInterface.h"

#include "GameUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameUIEvent);

class UCardManager;

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

	UPROPERTY(BlueprintCallable, Category = "Startup Events")
		FGameUIEvent OnToggleGameUI;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCardManager* CardManager;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* PlayerUI_Canvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UMessageManager* MessageManager;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* TurnButton;

public:
	//classes using this interface must implement UpdateUIPlayerStats
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUIPlayerStats(bool forceCleanVisualUpdate);
	//classes using this interface must implement UpdateUITurnState
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUITurnState(bool turnActive, EGameTurn turnState);
	//classes using this interface must implement UpdateUITurnState
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateUIGameTurnTime();
	//classes using this interface must implement UpdateUITurnState
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdatePlayerStateUI(int32 p1, int32 p2, int32 p3, int32 p4);

private:
	UFUNCTION()
		void ToggleGameUI();
	UFUNCTION()
		void OnTurnButtonClicked();

	bool bIsVisible;

protected:
	//virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	//virtual void NativeConstruct();
	//virtual void NativeDestruct();

};

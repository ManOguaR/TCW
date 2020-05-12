// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Enums.h"
#include "PlayerUIInterface.h"

#include "GameUI.generated.h"

class UCardManager;

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UGameUI : public UUserWidget, public IPlayerUIInterface
{
	GENERATED_BODY()

public:
	UCardManager* GetCardManager();

	void ToggleGameUI();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCardManager* CardManager;

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

};

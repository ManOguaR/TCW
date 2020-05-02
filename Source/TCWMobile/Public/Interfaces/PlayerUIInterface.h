// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "Enums.h"

#include "PlayerUIInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UPlayerUIInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API IPlayerUIInterface
{
	GENERATED_IINTERFACE_BODY()

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

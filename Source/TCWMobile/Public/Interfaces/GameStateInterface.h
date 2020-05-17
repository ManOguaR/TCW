// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "SystemEnums.h"

#include "GameStateInterface.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UGameStateInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API IGameStateInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	//classes using this interface must implement MatchEnd
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void MatchEnd(EEndGameResults endGameResult);
	//classes using this interface must implement MatchBegin
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void MatchBegin();
	//classes using this interface must implement ChangeActivePlayerTurn
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ChangeActivePlayerTurn(bool turnActive);
	//classes using this interface must implement RequestChangePlayerTurn
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RequestChangePlayerTurn();
};

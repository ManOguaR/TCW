// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "CardInteractionInterface.generated.h"

class ABoardUnit;

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UCardInteractionInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API ICardInteractionInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	//classes using this interface must implement MatchEnd
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TakesDamage(ABoardUnit* dealingUnit, int32 damageAttack, int32 damageHealth);
	//classes using this interface must implement MatchEnd
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnActivePlayerTurn();
	//classes using this interface must implement MatchBegin
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnEndActivePlayerTurn();
};

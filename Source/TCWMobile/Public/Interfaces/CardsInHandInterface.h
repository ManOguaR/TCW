// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "CardsInHandInterface.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UCardsInHandInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API ICardsInHandInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	//classes using this interface must implement MatchEnd
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RemoveCardFromHand(FName cardName, int32 index, bool removeAll);
	//classes using this interface must implement MatchBegin
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DrawCard(FName cardName, bool ignoreMaxCards, int32 numberOfCardsToDraw);
};

// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

#include "SystemEnums.h"

#include "DeckInterface.generated.h"

/**
 *
 */
UINTERFACE(BlueprintType)
class TCWMOBILE_API UDeckInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TCWMOBILE_API IDeckInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		TArray<FName> GetPlayerDeck();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool RemoveCardFromDeck(bool removeAll, int32 indexToRemove);
};

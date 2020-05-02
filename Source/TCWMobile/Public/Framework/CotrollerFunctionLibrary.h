// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AIGamePawn.h"
#include "AIPlayerController.h"
#include "TCWPlayerController.h"
#include "TCWPlayerState.h"

#include "CotrollerFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class TCWMOBILE_API UCotrollerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		static int32 GetControllerId(AController* controller);
	UFUNCTION(BlueprintPure)
		static ATCWPlayerController* GetTCWPlayerControllerReference(AController* controller, ATCWPlayerState* state, bool& isValid);
	UFUNCTION(BlueprintPure)
		static AAIPlayerController* GetAiControllerReference(AController* controller, AAIGamePawn* state, bool& isValid);

	UFUNCTION(BlueprintPure)
		static void GetControllersStateProfile(int32 controllerId, int32& health, int32& numCardsInHand, int32& cardsInDeck, int32& activeCards, int32& mana, int32& manaMax, int32& playerTurn, TArray<FName>& deck, TArray<FName>& cardsInHand, AActor* playerState);

};

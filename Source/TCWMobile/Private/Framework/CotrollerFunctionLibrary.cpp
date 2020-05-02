// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "CotrollerFunctionLibrary.h"

#include "../TCWMobile.h"
#include "AIGamePawn.h"
#include "AIPlayerController.h"

int32 UCotrollerFunctionLibrary::GetControllerId(AController* controller)
{
	if (ATCWPlayerController* playerController = Cast<ATCWPlayerController>(controller))
	{
		if (ATCWPlayerState* playerState = Cast<ATCWPlayerState>(playerController->PlayerState))
		{
			return playerState->GamePlayerId;
		}
	}
	else if (AAIPlayerController* aiController = Cast<AAIPlayerController>(controller))
	{
		if (AAIGamePawn* aiPawn = Cast<AAIGamePawn>(aiController->GetPawn()))
		{
			return aiPawn->GamePlayerId;
		}
	}
	UE_LOG(TCWLogErrors, Error, TEXT("Failed to get Controller"));
	return -1;
}

ATCWPlayerController* UCotrollerFunctionLibrary::GetTCWPlayerControllerReference(AController* controller, ATCWPlayerState* state, bool& isValid)
{
	state = nullptr;
	isValid = false;

	if (ATCWPlayerController* playerController = Cast<ATCWPlayerController>(controller))
	{
		if (ATCWPlayerState* playerState = Cast<ATCWPlayerState>(playerController->PlayerState))
		{
			state = playerState;
			isValid = true;
			return playerController;
		}
	}

	return nullptr;
}

AAIPlayerController* UCotrollerFunctionLibrary::GetAiControllerReference(AController* controller, AAIGamePawn* state, bool& isValid)
{
	state = nullptr;
	isValid = false;

	if (AAIPlayerController* aiController = Cast<AAIPlayerController>(controller))
	{
		if (AAIGamePawn* aiState = Cast<AAIGamePawn>(aiController->GetPawn()))
		{
			state = aiState;
			isValid = true;
			return aiController;
		}
	}

	return nullptr;
}

void UCotrollerFunctionLibrary::GetControllersStateProfile(int32 controllerId, int32& health, int32& numCardsInHand, int32& cardsInDeck, int32& activeCards, int32& mana, int32& manaMax, int32& playerTurn, TArray<FName>& deck, TArray<FName>& cardsInHand, AActor* playerState)
{
	//TODO:UCotrollerFunctionLibrary::GetControllersStateProfile
}

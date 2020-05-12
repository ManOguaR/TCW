// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 


#include "TCWGameState.h"
#include "..\TCWMobile.h"
#include "MiscFunctionLibrary.h"
#include "TCWGameMode.h"

#include "Kismet\GameplayStatics.h"
#include <TCWMobile\Public\System\SystemFunctionLibrary.h>

ATCWGameState::ATCWGameState(const FObjectInitializer& ObjectInitializer) : AGameState(ObjectInitializer)
{
	//Pre-22
	//EXE-6
	OnGameStart.AddDynamic(this, &ATCWGameState::GameStart);
	OnNotifyEndGameState.AddDynamic(this, &ATCWGameState::NotifyEndGameState);
}

void ATCWGameState::BeginPlay()
{
	//EXE-3
	if (HasAuthority())
	{
		UKismetSystemLibrary::Delay(this, 0.5f, FLatentActionInfo());
		//UMiscFunctionLibrary::Delay(this, 0.5f);

		GameModeRef = Cast<ATCWGameMode>(UGameplayStatics::GetGameMode(this));

	}
}

UBoardState* ATCWGameState::GetBoardState(int32 playerID)
{
	return nullptr;
}

void ATCWGameState::GameStart_Implementation()
{
	if (HasAuthority())
	{
		bGameActive = true;
		PlayerStateArray = GameModeRef->GetPlayerStateArray();
	}
}

void ATCWGameState::NotifyEndGameState_Implementation(EEndGameResults player1, EEndGameResults player2)
{
	if (HasAuthority())
	{
		bGameActive = false;
		UKismetSystemLibrary::K2_ClearTimerHandle(this, turnTimerRef);
		UKismetSystemLibrary::K2_ClearTimerHandle(this, gameTimerRef);

		TArray<AController*> controllers = GameModeRef->GetGameControllersArray();
		for (int a = 0; a < controllers.Num(); a++)
		{
			//TODO. GAME STATEINTERFACE MATCH END
			//controllers[a]->MatchEnd(a == 0 ? player1 : player2);
		}
		UKismetSystemLibrary::Delay(this, 5.0f, FLatentActionInfo());
		//UMiscFunctionLibrary::Delay(this, 5.0f);
		USystemFunctionLibrary::GetTCWGameInstance(this)->OnShowMainMenu.Broadcast();
	}
}

// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "TCWSpectator.generated.h"

/**
 * 
 */
UCLASS()
class TCWMOBILE_API ATCWSpectator : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	ATCWSpectator(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
};

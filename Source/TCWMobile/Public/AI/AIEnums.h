// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAITurnState : uint8
{
	AIState_Initialization		UMETA(DisplayName = "Initialization"),
	AIState_Execution			UMETA(DisplayName = "Execution"),
	AIState_Termination			UMETA(DisplayName = "Termination")
};

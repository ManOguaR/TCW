// Copyrigth (c) 2020 Softwar 19.23 NGS. All rigths reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CardStructs.h"
#include "Enums.h"

#include "CardFunctionLibrary.generated.h"

class ABoardUnit;

/**
 * 
 */
UCLASS()
class TCWMOBILE_API UCardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static ABoardUnit* SetupUnit(ABoardUnit* boardUnitRef, int32 owningPlayerId, FName cardName, ECardSet cardSet, FCardData dataStruct, bool setUnitDataFromStruct);
};
